// GeneratorComm.h: interface for the CGeneratorComm class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GENERATORCOMM_H
#define _GENERATORCOMM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialComm.h"

class CGeneratorComm : public CSerialComm  
{
// Methods
public:
	void setClear();
	// Constructor and destructor
	CGeneratorComm()
	{}
	CGeneratorComm(const char *port);
	virtual ~CGeneratorComm();
	// get the status of X-Ray generator
	int status() ;          // Is any alarm
	int isXRayOn();			  // Is X-Ray ON
	int getMA(void) ;			  // Get the current
	int getKV(void) ;             // Get the voltage

	// Setup the X-Ray generator
	void setXRay(bool bON);
	void setKV(int kv);
	void setMA(int ma);//float
	CSerialComm *m_SComm;

private:
	// read nLen bytes from the serial communication
	int readcomm(char *buffer, int nLen) ;

};

#endif // _GENERATORCOMM_H
