// Presentation.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "Presentation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPresentation

IMPLEMENT_DYNAMIC(CPresentation, CPropertySheet)

CPresentation::CPresentation(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPresentation::CPresentation(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&trackAttr);
	AddPage(&curveAttr);
}

CPresentation::~CPresentation()
{
}


BEGIN_MESSAGE_MAP(CPresentation, CPropertySheet)
	//{{AFX_MSG_MAP(CPresentation)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPresentation message handlers
