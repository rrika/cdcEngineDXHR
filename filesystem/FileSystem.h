#pragma once

namespace cdc {

struct FileRequest;
class FileReceiver;

class FileSystem {
public:
	virtual FileRequest *method_00(FileReceiver*, const char*, uint32_t);
	virtual void method_04();
	virtual bool fileExists(const char *path);
	virtual void method_0C();
	virtual void method_10();
	virtual void method_14();
	virtual void method_18();
	virtual void method_1C();
	virtual void setLanguageMask(uint32_t);
	virtual uint32_t getLanguageMask();
	virtual void method_24();
	virtual void method_28();
	virtual void method_2C();
	virtual void method_30();
	virtual void method_34();
	virtual void method_38();
	virtual void method_3C();
	virtual void method_40();
	virtual void method_44();
	virtual void method_48();
	virtual void method_4C();
	virtual void method_50();
	virtual void method_54();
	virtual void method_58();
	virtual void method_5C();
};

}
