#ifndef MUMPS_ROUTINE_H_
#define MUMPS_ROUTINE_H_

#include <string>

namespace mumps {

enum RoutineMappingStatus {
	kMapped,
	kFailure,
	kUndefined
};

class Routine {
public:
	friend class Lexer;
	Routine(): source(std::string()), status(kUndefined) {};
	virtual ~Routine() = default;
	Routine(const std::string &fname);
	/* disable copying */
	Routine(Routine &rhs) = delete;
	Routine& operator=(Routine &rhs) = delete;
	Routine(Routine &&rhs) noexcept : source(std::move(rhs.source)), status(rhs.status) { }
	Routine& operator=(Routine &&rhs) noexcept 
	{
		if (this != &rhs) {
			source = std::move(rhs.source);
			status = rhs.status;
		}
		rhs.source.clear();
		rhs.status = kUndefined;
		return *this;
	}
	void readFromFile(const std::string &fname);
	bool fail() const { return (status == kFailure); }
	size_t size() const { return source.length(); }
	void dump() const;
private:
	void mapFileToMemory(const char *fname);
private:
	std::string source;
	RoutineMappingStatus status;
};

} // namespace mumps 

#endif // MUMPS_ROUTINE_H_
