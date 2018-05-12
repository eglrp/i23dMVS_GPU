////////////////////////////////////////////////////////////////////
// Strings.h
//
// Distributed under the Boost Software License, Version 1.0
// (See http://www.boost.org/LICENSE_1_0.txt)

#ifndef __I23D_STRING_H__
#define __I23D_STRING_H__


// I N C L U D E S /////////////////////////////////////////////////

#include "Streams.h"


// D E F I N E S ///////////////////////////////////////////////////


namespace I23D {

// S T R U C T S ///////////////////////////////////////////////////

/// String class: enhanced std::string
class GENERAL_API String : public std::string
{
public:
	typedef std::string Base;

public:
	inline String() {}
	inline String(LPCTSTR sz) : Base(sz) {}
	inline String(const Base& str) : Base(str) {}
	inline String(size_t n, value_type v) : Base(n, v) {}
	inline String(LPCTSTR sz, size_t count) : Base(sz, count) {}
	inline String(LPCTSTR sz, size_t offset, size_t count) : Base(sz, offset, count) {}

	inline void Release() { return clear(); }
	inline bool IsEmpty() const { return empty(); }

	inline operator LPCTSTR() const { return c_str(); }

	String& Format(LPCTSTR szFormat, ...) {
		va_list args;
		va_start(args, szFormat);
		TCHAR szBuffer[2048];
		const int len(_vsntprintf(szBuffer, 2047, szFormat, args));
		if (len == -1 || len >= 2047) {
			*this = FormatStringSafe(szFormat, args);
			va_end(args);
		} else {
			va_end(args);
			*this = szBuffer;
		}
		return *this;
	}
	String& FormatSafe(LPCTSTR szFormat, ...) {
		va_list args;
		va_start(args, szFormat);
		const int cnt(_vsctprintf(szFormat, args));
		ASSERT(cnt >= 0);
		const size_t count((size_t)cnt+1);
		TCHAR* szBuffer(new TCHAR[count]);
		_vsntprintf(szBuffer, count, szFormat, args);
		va_end(args);
		*this = szBuffer;
		delete[] szBuffer;
		return *this;
	}
	static String FormatString(LPCTSTR szFormat, ...) {
		va_list args;
		va_start(args, szFormat);
		TCHAR szBuffer[2048];
		const int len(_vsntprintf(szBuffer, 2047, szFormat, args));
		if (len == -1 || len >= 2047) {
			const String str(FormatStringSafe(szFormat, args));
			va_end(args);
			return str;
		}
		va_end(args);
		return szBuffer;
	}
	static inline String FormatStringSafe(LPCTSTR szFormat, va_list args) {
		const int cnt(_vsctprintf(szFormat, args));
		ASSERT(cnt >= 0);
		const size_t count((size_t)cnt+1);
		TCHAR* szBuffer(new TCHAR[count]);
		_vsntprintf(szBuffer, count, szFormat, args);
		String str(szBuffer);
		delete[] szBuffer;
		return str;
	}

	inline void ToUpper(String& out) const {
		out.resize(size());
		std::transform(begin(), end(), out.begin(), ::towupper);
	}
	inline String ToUpper() const {
		String str;
		ToUpper(str);
		return str;
	}

	inline void ToLower(String& out) const {
		out.resize(size());
		std::transform(begin(), end(), out.begin(), ::towlower);
	}
	inline String ToLower() const {
		String str;
		ToLower(str);
		return str;
	}

	inline void	Save(OSTREAM& oStream) const {
		const WORD nSize = (WORD)size();
		oStream.write(&nSize, sizeof(WORD));
		oStream.write(c_str(), nSize);
	}
	inline void	Load(ISTREAM& oStream) {
		WORD nSize;
		oStream.read(&nSize, sizeof(WORD));
		if (nSize == 0) {
			clear();
			return;
		}
		char* pBuffer = new char[nSize];
		oStream.read(pBuffer, nSize);
		assign(pBuffer, nSize);
		delete[] pBuffer;
	}

	template <class T>
	static String ToString(const T& val) {
		std::ostringstream os;
		os << val;
		return os.str();
	}
	template <class T>
	static String ToStringHex(const T& val) {
		std::ostringstream os;
		os << std::hex << val;
		return os.str();
	}

	template <class T>
	static void FromString(const String& str, T& val) {
		std::istringstream is(str);
		is >> val;
	}
	template <class T>
	static T FromString(const String& str, const T& def) {
		T val(def);
		FromString(str, val);
		return val;
	}
	template <class T>
	static T FromString(const String& str) {
		T val;
		FromString(str, val);
		return val;
	}
	template <class T>
	inline void From(T& val) const {
		FromString(*this, val);
	}
	template <class T>
	inline T From(const T& def) const {
		T val(def);
		From(val);
		return val;
	}
	template <class T>
	inline T From() const {
		T val;
		From(val);
		return val;
	}

	static int CompareAlphabetically(const void* elem, const void* key) { return _tcscmp(((const String*)elem)->c_str(), ((const String*)key)->c_str()); }
	static int CompareAlphabeticallyInv(const void* elem, const void* key) { return _tcscmp(((const String*)key)->c_str(), ((const String*)elem)->c_str()); }

#ifdef _USE_BOOST
protected:
	// implement BOOST serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int /*version*/) {
		ar & boost::serialization::base_object<Base>(*this);
	}
#endif
};
/*----------------------------------------------------------------*/

} // namespace I23D


namespace std {
//namespace tr1 {
// Specializations for unordered containers
template <> struct hash<I23D::String> : public hash<string>{};
//} // namespace tr1
template <> struct equal_to<I23D::String> : public equal_to<string>{};
} // namespace std

#endif // __I23D_STRING_H__