

#ifndef ___m_typeS___
#define ___m_typeS___

namespace mray{

//typedef unsigned	char	unsigned char;
//typedef signed		char	schar;

typedef unsigned	int		uint;
typedef signed		int		sint;

//typedef unsigned	short	unsigned short;
typedef signed		short	sshort;

typedef unsigned	long	ulong;
typedef unsigned	long long	ullong;
typedef signed		long	slong;
//typedef unsigned char utf8;
typedef uint utf32;


#define mT(X) (X)

#ifdef UNICODE

typedef  wchar_t mchar;

#else

typedef char mchar;

#endif

};//mray



#endif