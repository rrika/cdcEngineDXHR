#pragma once
#include <cstdint>

namespace cdc {

class HandleData;

class HandleOwner { // line 43
	uint16_t m_handle; // line 73
public:
	HandleOwner(); // line 48
	~HandleOwner(); // line 51
	bool IsInitialized() const { return m_handle != 0; } // line 55
	void CreateHandle(void *object); // line 59
	operator HandleData*() const; // line 63
private:
	HandleOwner(HandleOwner const&); // line 69
	HandleOwner& operator=(HandleOwner const&); // line 70
};

template <typename T>
class Handle { // line 88
protected:
	mutable HandleData *m_handle; // line 113
public:
	Handle(); // line 572
	Handle(T*); // line 581
	Handle(Handle const&); // line 596
	~Handle(); // line 589
	Handle& operator=(Handle const&); // line 604
	T& operator=(T const*); // line 615
	T *Get() const; // line 631
	operator T*() const; // line 647
	T *operator->() const; // line 653
	T *operator*() const; // line 660

	bool isSet() const { return !!m_handle; } // HACK
};

class RCObject { // line 141
	uint16_t m_refCount;
	HandleOwner m_handleOwner;

protected:
	RCObject() : m_refCount(0) {} // line 386
public:
	RCObject(RCObject const&); // line 391
	HandleData *GetHandleData(); // line 403
	uint32_t GetRefCount(); // line 410
	void AddReference(); // line 415
	void RemReference(); // line 426

	virtual ~RCObject(); // line 88
	virtual void Remove(); // line 93
};

template <typename T>
class RCPtr { // line 210
	T *m_object;

public:
	RCPtr(T *t); // line 444
	RCPtr(const RCPtr& other); // line 452
	~RCPtr(); // line 469
	RCPtr& operator=(T* other) const; // line 487
	RCPtr& operator=(const RCPtr& other) const; // line 512
	T* Get() const; // line 525
	operator T*() const; // line 531
	T *operator->() const; // line 537
};

void InitHandlePool(uint32_t count); // line 280
void FreeHandlePool();
bool IsHandlePoolValid(); // line 282
uint32_t GetHandleCount();

class HandleData { // line 277, isn't that impossible?
public:
	void *Get() const { // line 289
		// guessed
		return m_object;
	}

	void AddReference() { // line 292
		m_refCount++;
	}

private:
	friend class HandleOwner;
	friend void InitHandlePool(uint32_t count);
	friend bool IsHandlePoolValid();

	HandleData() : m_object(nullptr), m_refCount(0) {} // line 297
	HandleData(HandleData const&); // line 298
	HandleData& operator=(HandleData const&); // line 299

	static uint16_t Create(void *); // line 301
	static void Destroy(HandleData*); // line 302

	void *m_object; // line 305
	int32_t m_refCount; // line 308
	static HandleData *s_handles; // line 311
	static HandleData *s_free; // line 312

public:
	void RemReference() { // line 355
		if (--m_refCount == 0) {
			m_object = (void*)s_free;
			s_free = this;
		}
	}

};

// ------------------------------------------------------------------------- //

inline HandleOwner::HandleOwner() : m_handle(0) {} // line 345

inline HandleOwner::~HandleOwner() { // ?
	if (m_handle) {
		HandleData::s_handles[m_handle].m_object = nullptr;
		HandleData::s_handles[m_handle].RemReference();
	}
}

inline void HandleOwner::CreateHandle(void *object) { // line 364
	m_handle = HandleData::Create(object);
	HandleData::s_handles[m_handle].AddReference();
}

inline HandleOwner::operator HandleData*() const { // line 377
	// can't determine if there should be a null-check here or not
	if (m_handle)
		return &HandleData::s_handles[m_handle];
	return nullptr;
}

// ------------------------------------------------------------------------- //

// inline RCObject::RCObject(); // line 386

// inline RCObject::RCObject(RCObject const&); // line 391

inline HandleData *RCObject::GetHandleData() { // line 403
	if (!m_handleOwner.IsInitialized()) // line 405
		m_handleOwner.CreateHandle(this); // line 406

	return (HandleData*)m_handleOwner;
}

inline uint32_t RCObject::GetRefCount() { // line 410
	// guess
	return m_refCount;
}

inline void RCObject::AddReference() { // line 415
	m_refCount++;
}

inline void RCObject::RemReference() { // line 426
	if (--m_refCount == 0)
		Remove();
}

// ------------------------------------------------------------------------- //

template <typename T>
inline RCPtr<T>::RCPtr(T *t) : m_object(t) { // line 444
	m_object->AddReference();
}

template <typename T>
inline RCPtr<T>::RCPtr(const RCPtr& other) : m_object(other.Get()) { // line 452
	m_object->AddReference();
}

template <typename T>
inline RCPtr<T>::~RCPtr() { // line 469
	if (m_object)
		m_object->RemReference(); // line 479
}

template <typename T>
inline RCPtr<T>& RCPtr<T>::operator=(T* other) const { // line 487
	if (other)
		other->AddReference(); // line 502
	if (m_object)
		m_object->RemReference(); // line 506
}

template <typename T>
inline RCPtr<T>& RCPtr<T>::operator=(const RCPtr& other) const { // line 512
	*this = other.Get();
}

template <typename T>
inline T* RCPtr<T>::Get() const { return m_object; } // line 525

template <typename T>
inline RCPtr<T>::operator T*() const { return m_object; } // line 531

template <typename T>
inline T *RCPtr<T>::operator->() const { return m_object; } // line 537

// ------------------------------------------------------------------------- //

template <typename T>
inline Handle<T>::Handle() : m_handle(nullptr) { // line 572
	if (!IsHandlePoolValid())
		InitHandlePool(0x2000);
}

template <typename T>
inline Handle<T>::Handle(T *t) { // line 581
	HandleData *hd = nullptr;
	if (t)
		hd = static_cast<RCObject*>(t)->GetHandleData();

	m_handle = hd;
	if (hd)
		hd->AddReference();
}

template <typename T>
inline Handle<T>::~Handle() { // line 589
	if (m_handle)
		m_handle->RemReference();
}

template <typename T>
inline Handle<T>::Handle(Handle const& other) : Handle(other.Get()) {} // line 596

template <typename T>
inline Handle<T>& Handle<T>::operator=(Handle const& other) { // line 604
	if (m_handle)
		m_handle->RemReference();
	m_handle = other.m_handle;
	if (m_handle)
		m_handle->AddReference();
	return *this;
}

// template <typename T>
// inline T& Handle<T>::operator=(T const*); // line 615

template <typename T>
inline T *Handle<T>::Get() const { // line 631
	auto *t = reinterpret_cast<T*>(m_handle->Get());
	if (!t) {
		m_handle->RemReference();
		m_handle = nullptr;
	}
	return t;
}

template <typename T>
inline Handle<T>::operator T*() const {  // line 647
	// guess
	return Get();
}

template <typename T>
inline T *Handle<T>::operator->() const {  // line 653
	// guess
	return Get();
}

template <typename T>
inline T *Handle<T>::operator*() const {  // line 660
	// guess
	return Get();
}

}
