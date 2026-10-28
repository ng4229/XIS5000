// NSEException.cpp: implementation of the CNSEException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NSEException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNSEException::CNSEException()
: m_explanation("")
{
}

CNSEException::CNSEException(const char *explanation)
: m_explanation(explanation)
{

}


CNSEException::CNSEException(const CNSEException &de)
{
	// Copy the description
	m_explanation = de.getDescription ();
}

// DESTRUCTORS ////////////////////////////////////////////////////////////////

CNSEException::~CNSEException()
{

}

// MEMBER FUNCTIONS ///////////////////////////////////////////////////////////

const char *CNSEException::getDescription () const
{
	return m_explanation.c_str ();
}

// OVERLOADED OPERATORS ///////////////////////////////////////////////////////

CNSEException & CNSEException::operator =(const CNSEException &de)
{
	// Prevent assign itself
	if (this != &de)
	{
		m_explanation = de.getDescription();
	}

	// return itself
	return *this;
}

