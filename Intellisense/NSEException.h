// NSEException.h: interface for the CNSEException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSEEXCEPTION_H__EAEAAD90_731E_432D_A66F_4C72AD66A5D1__INCLUDED_)
#define AFX_NSEEXCEPTION_H__EAEAAD90_731E_432D_A66F_4C72AD66A5D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CNSEException  
{
// Attributes
public:
	// Explanation
	const char *getDescription(void) const;

// Methods
public:
	// Constructor
	CNSEException();
	CNSEException(const char *explaination);
	CNSEException(const CNSEException &de);

	// Destructor
	virtual ~CNSEException();

	// The assignment operator =
	CNSEException &operator =(const CNSEException &de);

// Implementation
private:
	std::string m_explanation;
};

#endif // !defined(AFX_NSEEXCEPTION_H__EAEAAD90_731E_432D_A66F_4C72AD66A5D1__INCLUDED_)

