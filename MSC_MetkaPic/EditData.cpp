// EditData.cpp : implementation file
//

#include "stdafx.h"
#include "MFTMonitor.h"
#include "EditData.h"
#include "threads.h"
#include "_Recordset.h"
//#include "field.h"
//#include "fields.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditData dialog

CString Provider[]={"Provider=MSDASQL.1;Extended Properties=",
					"Provider=MSDASQL.1;",	
					"Provider=Microsoft.Jet.OLEDB.4.0;Data Source="
					
};

CEditData::CEditData(CWnd* pParent /*=NULL*/)
	: CDialog(CEditData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditData)
	m_dA = 1.0;
	m_dB = 0.0;
	m_sName = _T("");
	//}}AFX_DATA_INIT
}


void CEditData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditData)
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_cMessage);
	DDX_Control(pDX, IDC_COMBO_STEP, m_cStep);
	DDX_Control(pDX, IDC_COMBO_DIRECTION, m_cDirection);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_COMBO_TABLE, m_cTable);
	DDX_Control(pDX, IDC_COMBO_FIELD, m_cField);
	DDX_Control(pDX, IDC_COMBO_DATA, m_cData);
	DDX_Control(pDX, IDC_ADODC1, m_cAdodc);
	DDX_Control(pDX, IDC_DATAGRID1, m_cDataGrid);
	DDX_Text(pDX, IDC_EDIT_A, m_dA);
	DDX_Text(pDX, IDC_EDIT_B, m_dB);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditData, CDialog)
	//{{AFX_MSG_MAP(CEditData)
	ON_BN_CLICKED(IDC_BUTTON1, OnBrowse)
	ON_WM_CLOSE()
	ON_CBN_SELENDOK(IDC_COMBO_TABLE, OnSelendokComboTable)
	ON_CBN_SELENDOK(IDC_COMBO_FIELD, OnSelendokComboField)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeListField)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_REPORT, OnReport)
	ON_CBN_SELENDOK(IDC_COMBO_DIRECTION, OnSelendokComboDirection)
	ON_CBN_SELENDOK(IDC_COMBO_STEP, OnSelendokComboStep)
	ON_BN_CLICKED(IDC_REPORT2, OnSelectMSC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditData message handlers

BOOL CEditData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString cs = "Edit Database: ";
	CString str;
	
	if(!strcmp(CurName,"")){
		m_sBrowse = "C:\\a2.mdb";
		//m_sBrowse = "a2.mdb";
	}

	else 
		m_sBrowse = CurName;
	cs = cs + m_sBrowse;
	SetWindowText(cs);
	m_cDataGrid.SetCaption(m_sBrowse);
	m_iRecOpen = 0;
	

	OnBrowse();
	m_sName = m_sBrowse;
	m_sName.TrimRight(".mdb");
	m_sName = m_sName + "_R.las";
	UpdateData(FALSE);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
   
}

void CEditData::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	CString cs = "Edit Database: ";
	CFileDialog m_filedlg1(TRUE,"mdb",NULL,OFN_EXPLORER,
				  "Access Database Files (*.mdb)|*.mdb||");	
		if(m_filedlg1.DoModal()==IDOK)
		{
			m_sBrowse = m_filedlg1.GetPathName();
			cs = cs + m_sBrowse;
			SetWindowText(cs);
			m_cDataGrid.SetCaption(m_sBrowse);
			m_cAdodc.SetConnectionString(Provider[2]+m_sBrowse);
			m_cAdodc.SetCommandType(2);
			m_cAdodc.SetRecordSource("DataTable0");
			m_cAdodc.Refresh();
			if(!m_cAdodc.GetRecordset().GetRecordCount())
				return;
			
			ListAccessTables(m_sBrowse);
			m_cTable.SetCurSel(0);
			m_cField.SetCurSel(0);
			m_cData.SetCurSel(0);
		}
		else {
			//m_sBrowse = "c:\\metka\\mft3\\demo.mdb";
			//m_sBrowse = "c:\\Inclinometre\\inc.mdb";
			m_sBrowse = //"c:\\metka\\mft3\\demo.mdb";
				"C:\\a2.mdb";
			cs = cs + m_sBrowse;
			SetWindowText(cs);
			m_cDataGrid.SetCaption(m_sBrowse);
			m_cAdodc.SetConnectionString(Provider[2]+m_sBrowse);
			m_cAdodc.SetCommandType(2);
			m_cAdodc.SetRecordSource("DataTable0");
			m_cAdodc.Refresh();
			ListAccessTables(m_sBrowse);
			m_cTable.SetCurSel(0);
			m_cField.SetCurSel(0);
			m_cData.SetCurSel(0);
		}
		m_sName = m_sBrowse;
		m_sName.TrimRight(".mdb");
		m_sName = m_sName + "_R.las";
		UpdateData(FALSE);
}

void CEditData::ListAccessTables(CString cs)
{
	m_cTable.ResetContent();

	_ConnectionPtr Con = NULL;
	
	VARIANT varV;
	CString csTemp,str;
		
	try
	{	
		Con.CreateInstance(__uuidof(Connection));
		Con->Open(_bstr_t(Provider[2]+cs),"","",NULL);

		m_rRs = Con->OpenSchema(adSchemaTables);
		m_iRecOpen = 1;
							
		while(VARIANT_TRUE != m_rRs->GetAdoEOF()){
			csTemp = "";
			varV = m_rRs->Fields->GetItem("TABLE_TYPE")->Value;
			if(varV.vt == VT_BSTR)	csTemp = varV.bstrVal;
			
			if( csTemp == _T("TABLE")){
				varV = m_rRs->Fields->GetItem("TABLE_NAME")->Value;
				csTemp = varV.bstrVal;
				m_cTable.AddString(csTemp);
			}
		
			m_rRs->MoveNext();
		}

	}
	catch(_com_error &err)
	{
		DumpError(err);
	}
	m_cTable.GetLBText(0,csTemp);
	ListAccessFields(csTemp);
	m_sFrom = "Select * From ["+csTemp+"]";
	m_cField.GetLBText(0,str);
	ListAccessData(str);
	m_cData.SetCurSel(0);


	ListAccessListFields(csTemp);
    ListAccessCombobox();
}


_RecordsetPtr CEditData::GetRecorset(CString cs, long *nfields, long *nrecords)
{
	_ConnectionPtr Con = NULL;
	FieldsPtr fldLoop;
	
	VARIANT varV;
	CString csTemp,str,sz,ss;
	
	_variant_t Index,varI;
    Index.vt = VT_I2;
	_bstr_t varS;	
	
	int i=0;

	try
	{	
		Con.CreateInstance(__uuidof(Connection));
		Con->Open(_bstr_t(Provider[2]+cs),"","",NULL);

		m_rRs = Con->OpenSchema(adSchemaTables);
									
		while(VARIANT_TRUE != m_rRs->GetAdoEOF()){
			csTemp = "";
			varV = m_rRs->Fields->GetItem("TABLE_TYPE")->Value;
			if(varV.vt == VT_BSTR)	csTemp = varV.bstrVal;
			
			if( csTemp == _T("TABLE")){
				varV = m_rRs->Fields->GetItem("TABLE_NAME")->Value;
				if(!i) ss = varV.bstrVal;
				i++;
				
			}
			
			m_rRs->MoveNext();
		}

	}
	catch(_com_error &err)
	{
		DumpError(err);
	}

	CString sql(_T("Select * From ["+ss+"]"));

	try	{	
		m_rRs.CreateInstance(__uuidof(Recordset));
		m_rRs->Open(_bstr_t(sql),
				 _bstr_t(Provider[2]+cs),
				 adOpenForwardOnly,
				 adLockReadOnly,
				 adCmdUnknown);

	
		fldLoop = m_rRs->GetFields();

			
		long ll= 0;
		int i1;
		m_rRs->MoveFirst();
		while(VARIANT_TRUE != m_rRs->GetAdoEOF()){
			ll++;
			m_rRs->MoveNext();
		}
		
		*nfields = fldLoop->GetCount();
		*nrecords = ll;
			
		for ( i1 = 0; i1 < (int)fldLoop->GetCount(); i1++){
            Index.iVal = i1;
            if (fldLoop->GetItem(Index)->GetAttributes()){
                varS = fldLoop->GetItem(Index)->GetName();
				
            }
        }		
	}
	catch(_com_error &err)
	{
		DumpError(err);
	}
	

	return m_rRs;
}


void CEditData::ListAccessFields(CString cs)
{
	m_cField.ResetContent();
	
	if(m_iRecOpen) m_rRs->Close();
	
	CString sql(_T("Select * From ["+cs+"]"));
		
	FieldsPtr fldLoop;
	CString sz;
	
	_variant_t Index,varI;
    Index.vt = VT_I2;
	_bstr_t varS;
	
	try	{	
		m_rRs.CreateInstance(__uuidof(Recordset));
		m_rRs->Open(_bstr_t(sql),
				 _bstr_t(Provider[2]+m_sBrowse),
				 adOpenForwardOnly,
				 adLockOptimistic,
				 adCmdUnknown);

		fldLoop = m_rRs->GetFields();
			
		long ll=0;
		int i1;
		m_rRs->MoveFirst();
		while(VARIANT_TRUE != m_rRs->GetAdoEOF()){
			ll++;
			m_rRs->MoveNext();
		}
		
		//m_sNumber.Format("of total %ld records",ll);
		//UpdateData(FALSE);
			
		for ( i1 = 0; i1 < (int)fldLoop->GetCount(); i1++){
            Index.iVal = i1;
            if (fldLoop->GetItem(Index)->GetAttributes()){
                varS = fldLoop->GetItem(Index)->GetName();
				m_cField.AddString((LPCTSTR)varS);
            }
        }
		
	}
	catch(_com_error &err)
	{
		DumpError(err);
	}
}


void CEditData::ListAccessData(CString cs)
{
	m_cData.ResetContent();

	if(m_iRecOpen) m_rRs->Close();
	InitRecordset(m_sFrom);
		
	CString sz;
	_variant_t Index,varV;
    Index.vt = VT_I2;
	
		
	try	{	
		for(int i=0;i< (int)m_rRs->GetFields()->GetCount();i++){
			sz = (LPCTSTR)m_rRs->GetFields()->GetItem((long)i)->GetName();
			if(sz == cs) {
				Index.iVal = i;
				break;
			}
		}
		int j = 1;
				
		while(VARIANT_TRUE != m_rRs->GetAdoEOF()){
			
			varV = m_rRs->GetFields()->GetItem(Index)->Value;
			
			if(varV.vt == VT_BSTR){
				char ss[256];
				
				//m_rRs->Fields->GetItem(Index)->GetDataFormat().GetFont

				WideCharToMultiByte(CP_ACP,0,varV.bstrVal,-1,ss,256,NULL,NULL);
				m_cData.AddString(ss);
			}
			else if(varV.vt == VT_I4){
				sz.Format("%ld",varV.lVal);
				m_cData.AddString(sz);
			}
			else if(varV.vt == VT_I2){
				sz.Format("%d",varV.lVal);
				m_cData.AddString(sz);
			}
			else if(varV.vt == VT_DATE){
				COleDateTime time = varV.date;
				sz = time.Format("%d/%m/%Y");
				m_cData.AddString(sz);

			}
			else if(varV.vt == VT_CY){
				
				sz.Format("%g",varV.cyVal.int64/10000.0);
				m_cData.AddString(sz);

			}
			else if(varV.vt == (VT_ARRAY|VT_UI1)){
				BYTE HUGEP *buf = NULL;
				_variant_t varI;
				long lsize = 0;				
				lsize = m_rRs->Fields->GetItem(Index)->GetActualSize();
								
				if(lsize){
					VariantInit(&varI);
					varI = m_rRs->Fields->GetItem(Index)->GetChunk(lsize);
					sz.Format("Picture%d",j);
					m_cData.AddString(sz);
					j++;					
				}
			}
			else if(varV.vt == VT_R4){
				sz.Format("%.3f",varV.fltVal);
				m_cData.AddString(sz);
			}
			else if(varV.vt == VT_R8){
				sz.Format("%.3f",varV.dblVal);
				m_cData.AddString(sz);
			}
			else {
				
				sz.Format("%x",varV.vt);
				m_cData.AddString(sz);

			}
			
			m_rRs->MoveNext();
			
		}
			
	}
	catch(_com_error &err)
	{
		DumpError(err);
	}
		
}

void CEditData::ListAccessListFields(CString cs)
{
	m_cList.ResetContent();
	
	if(m_iRecOpen) m_rRs->Close();
	
	CString sql(_T("Select * From ["+cs+"]"));
		
	FieldsPtr fldLoop;
	CString sz;
	
	_variant_t Index,varI;
    Index.vt = VT_I2;
	_bstr_t varS;
	
	try	{	
		m_rRs.CreateInstance(__uuidof(Recordset));
		m_rRs->Open(_bstr_t(sql),
				 _bstr_t(Provider[2]+m_sBrowse),
				 adOpenForwardOnly,
				 adLockReadOnly,
				 adCmdUnknown);

		fldLoop = m_rRs->GetFields();
			
		long ll=0;
		int i1;
		m_rRs->MoveFirst();
		while(VARIANT_TRUE != m_rRs->GetAdoEOF()){
			ll++;
			m_rRs->MoveNext();
		}
		
		//m_sNumber.Format("of total %ld records",ll);
		//UpdateData(FALSE);
			
		for ( i1 = 0; i1 < (int)fldLoop->GetCount(); i1++){
            Index.iVal = i1;
            if (fldLoop->GetItem(Index)->GetAttributes()){
                varS = fldLoop->GetItem(Index)->GetName();
				m_cList.AddString((LPCTSTR)varS);
            }
        }
		
	}
	catch(_com_error &err)
	{
		DumpError(err);
	}		
}

int CEditData::InitRecordset(CString cs)
{
	CString sql(_T(cs));
		
	try	{	
		m_rRs.CreateInstance(__uuidof(Recordset));
		m_rRs->Open(_bstr_t(sql),
				 _bstr_t(Provider[2]+m_sBrowse),
				   adOpenForwardOnly,
				 adLockReadOnly,
				 adCmdUnknown);
     
		return TRUE;
	}
	catch(_com_error &err)
	{
		DumpError(err);
	}
	return FALSE;	
		
}

void CEditData::DumpError(_com_error &e)
{
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	CString str;
	str.Format("\tCode = %08lx",e.Error());
	str += " Msg: ";
	str += e.ErrorMessage();
	str += " Source: ";
	str += bstrSource;
	str += " Discription: ";
	str += bstrDescription;
	AfxMessageBox(str);
}

void CEditData::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	
	CDialog::OnClose();
}

void CEditData::OnSelendokComboTable() 
{
	// TODO: Add your control notification handler code here
	CString cs,str;
	//UpdateData(TRUE);
	m_cTable.GetLBText(m_cTable.GetCurSel(),cs);
	if(!cs.GetLength()) return;
	
	//m_sCaption = m_sBrowse+":"+cs;
	m_sFrom = "Select * From ["+cs+"]";
	
	//m_sWhere = "";
    //m_sLike = "";
    //m_sQuery = "";
    //m_sSQL = m_sFrom + m_sWhere + m_sLike;
    //UpdateData(FALSE);
	
	m_cAdodc.SetConnectionString(Provider[2]+m_sBrowse);
	m_cAdodc.SetCommandType(2);
	m_cAdodc.SetRecordSource(cs);
	m_cAdodc.Refresh();
	if(!m_cAdodc.GetRecordset().GetRecordCount())
				return;
			
			
	ListAccessFields(cs);
	m_sFrom = "Select * From ["+cs+"]";
	m_cField.GetLBText(0,str);
	ListAccessData(str);
	m_cField.SetCurSel(0);
	m_cData.SetCurSel(0);

	//if(!m_iFlag) OnSqlDb() ;
}

void CEditData::OnSelendokComboField() 
{
	// TODO: Add your control notification handler code here
	CString cs;
	//OnSelendokComboTable() ;
	m_cField.GetLBText(m_cField.GetCurSel(),cs);
	
	if(!cs.GetLength()) return;
	//m_sTemp = cs;
	
	//m_sCaption = m_sOpenDB+":"+cs;
	if(m_sLike.GetLength()>1) 
		m_sWhere = " Where ["+cs+"] Like \"%";
	else m_sWhere = " Where ["+cs+"] Like \"%%\"";
	
	//m_sSQL = m_sFrom + m_sWhere + m_sLike;
	//UpdateData(FALSE);

	CString sz;
	m_cTable.GetLBText(m_cTable.GetCurSel(),sz);
	m_sFrom = "Select * From ["+sz+"]"; 
	
	ListAccessData(cs);
	m_cData.SetCurSel(0);
	//if(!m_iFlag) OnSqlDb() ;
}

void CEditData::OnSelchangeListField() 
{
	// TODO: Add your control notification handler code here

	 CString cs;
     int index;

     index = m_cList.GetCurSel();
	 m_cList.GetText(index,cs);
	 
	 if(!cs.GetLength()) return;
	//m_sTemp = cs;
	 m_id[index]=index;

}

void CEditData::ListAccessCombobox()
{
	//Add string to combobox  
    m_cDirection.ResetContent();
    m_cDirection.AddString("Up");
    m_cDirection.AddString("Down");
    m_cStep.ResetContent();
	m_cStep.AddString("0.01");
    m_cStep.AddString("0.1");
    m_cStep.AddString("1");
    m_cStep.AddString("None");
  
}

void CEditData::OnSqlDb(CString cs) 
{
	// TODO: Add your control notification handler code here
	if(!m_sSQL.GetLength()) return;
	m_cAdodc.SetConnectionString(Provider[2]+m_sBrowse);
	m_cAdodc.SetCommandType(1);
	m_cAdodc.SetRecordSource(cs);
	m_cAdodc.Refresh();
	//m_sNumber.Format("of total %ld records",m_cDataGrid.GetApproxCount());
	UpdateData(FALSE);
	//OnClickDatagrid(); 
		
}

void CEditData::OnExport() 
{
	// TODO: Add your control notification handler code here
    CString fname;
	CFile m_fInfo;
    char* temp;


	_variant_t Index,varV;
    Index.vt = VT_I2;
	CString strOutput =" ";
	_bstr_t varS;
   
    
	CString sr;
    CString dx;
	CString m_sql;
	//OnSelendokComboDirection() ;
	m_cDirection.GetLBText(m_cDirection.GetCurSel(),dx);
	
	if(!dx.GetLength()) return;
	//m_sTemp = cs;

	//m_sCaption = m_sOpenDB+":"+cs;

    CString cs;
	CString sFrom2;
    CString sFrom1;
    CString sFrom3;
	CString sf;

    m_cTable.GetLBText(m_cTable.GetCurSel(),cs);

    // selected item CListBox
	CListBox *team = (CListBox*) GetDlgItem(IDC_LIST1);
	if(team->GetSelCount()>0){
		    int selected[NUMCHANNEL];
            int numSelected = team->GetSelItems(NUMCHANNEL,selected);

			for(int i=0;i<numSelected;i++){
				CString buffer;
				team->GetText(selected[i],buffer);
			
                 sFrom2= sFrom2 + "["+buffer+"],";
				  
					 sf = sf + "["+buffer+"]" + "\t"  ;
			}
		}
			
	sFrom2.TrimRight(',');
	
	if(!cs.GetLength()) return;
	
	//m_sCaption = m_sBrowse+":"+cs;

	sFrom1 = "Select ";
    sFrom3 =" From ["+cs+"]";
    m_sFrom  = sFrom1 + sFrom2 + sFrom3;
	
	
	m_sWhere = "";
	m_sLike = "";
	m_sQuery = "";
   
	
	//m_sSQL = m_sFrom + m_sWhere + m_sLike;
	//UpdateData(FALSE);

	m_sql = m_sFrom + m_sWhere ;//+ m_sOrder ;
	UpdateData(FALSE);
	

    temp = new char[100];
    GetSystemDirectory (temp, 100);
    fname = temp;
    fname = "c:\\example.txt";
   
    m_fInfo.SetFilePath (fname);

	//OnSelendokComboStep() ;
    CString dz;
	m_cStep.GetLBText(m_cStep.GetCurSel(),dz);

   
    if (!m_fInfo.Open(fname, CFile::modeCreate | CFile::modeWrite))
        MessageBox ("Unsuccessful");

	if(m_iRecOpen) m_rRs->Close();
	InitRecordset(m_sql);

    try	{	

		
	    int k =0;
		strOutput += sf ;
		strOutput += "\r\n";
		while(VARIANT_TRUE != m_rRs->GetAdoEOF()){
				for(int i=0;i< (int)m_rRs->GetFields()->GetCount();i++){
					sr = (LPCTSTR)m_rRs->GetFields()->GetItem((long)i)->GetName();
					CString sz;
					if(sr == "DEPTH") {
						 int j = 1;
					  	 Index.iVal = i;
						 varV = m_rRs->GetFields()->GetItem(Index)->Value;

						if(varV.vt == VT_BSTR){
							char ss[256];
					
							//m_rRs->Fields->GetItem(Index)->GetDataFormat().GetFont
							WideCharToMultiByte(CP_ACP,0,varV.bstrVal,-1,ss,256,NULL,NULL);
						}
						else if(varV.vt == VT_I4){
							sz.Format("%ld",varV.lVal);
						}
						else if(varV.vt == VT_I2){
							sz.Format("%d",varV.lVal);
						}
						else if(varV.vt == VT_DATE){
							COleDateTime time = varV.date;
							sz = time.Format("%d/%m/%Y");	
						}
						else if(varV.vt == VT_CY){
							sz.Format("%g",varV.cyVal.int64/10000.0);
						}
						else if(varV.vt == (VT_ARRAY|VT_UI1)){
							BYTE HUGEP *buf = NULL;
							_variant_t varI;
							long lsize = 0;				
							lsize = m_rRs->Fields->GetItem(Index)->GetActualSize();
								
							if(lsize){
								VariantInit(&varI);
								varI = m_rRs->Fields->GetItem(Index)->GetChunk(lsize);
								sz.Format("Picture%d",j);
								j++;					
							}
						}
			
						if(dz == "0.01") {
							if(varV.vt == VT_R4){
								sz.Format("%.2f",varV.fltVal);
							}
							else if(varV.vt == VT_R8){
								sz.Format("%.2f",varV.dblVal);
							}
							else {
								sz.Format("%x",varV.vt);
							}
						}else if (dz == "0.1"){
							if(varV.vt == VT_R4){
								sz.Format("%.1f",varV.fltVal);
							}
							else if(varV.vt == VT_R8){
								sz.Format("%.1f",varV.dblVal);
							}
							else {
								sz.Format("%x",varV.vt);
							}
						}else if(dz == "1"){
							if(varV.vt == VT_R4){
								sz.Format("%.0f",varV.fltVal);
							}
							else if(varV.vt == VT_R8){
								sz.Format("%.0f",varV.dblVal);
							}
							else {
								sz.Format("%x",varV.vt);
							}		
						}else if (dz== "None" ){
							if(varV.vt == VT_R4){
								sz.Format("%.3f",varV.fltVal);
							}
							else if(varV.vt == VT_R8){
								sz.Format("%.3f",varV.dblVal);
							}
							else {
								sz.Format("%x",varV.vt);
							}		
						}
  					//break;
					}else  {
							int j = 1;
							//CString sz;
							Index.iVal = i;
							varV = m_rRs->GetFields()->GetItem(Index)->Value;
			
							if(varV.vt == VT_BSTR){
								  char ss[256];
				
								//m_rRs->Fields->GetItem(Index)->GetDataFormat().GetFont

								WideCharToMultiByte(CP_ACP,0,varV.bstrVal,-1,ss,256,NULL,NULL);
							}
							else if(varV.vt == VT_I4){
								sz.Format("%ld",varV.lVal);
							}
							else if(varV.vt == VT_I2){
								sz.Format("%d",varV.lVal);
							}
							else if(varV.vt == VT_DATE){
								COleDateTime time = varV.date;
								sz = time.Format("%d/%m/%Y");	
							}
							else if(varV.vt == VT_CY){
								sz.Format("%g",varV.cyVal.int64/10000.0);
							}
							else if(varV.vt == (VT_ARRAY|VT_UI1)){
								BYTE HUGEP *buf = NULL;
								_variant_t varI;
								long lsize = 0;				
								lsize = m_rRs->Fields->GetItem(Index)->GetActualSize();
								
								if(lsize){
									VariantInit(&varI);
									varI = m_rRs->Fields->GetItem(Index)->GetChunk(lsize);
									sz.Format("Picture%d",j);
									j++;					
								}
							}
							else if(varV.vt == VT_R4){
								sz.Format("%.6f",varV.fltVal);
							}
							else if(varV.vt == VT_R8){
								sz.Format("%.5f",varV.dblVal);
							}
							else {
								sz.Format("%x",varV.vt);
							}
						}
					strOutput +=  sz + "\t"  ;
					}
		    
				strOutput += "\r\n";
				
				m_rRs->MoveNext();	
		}		
	}
	catch(_com_error &err)
	{
		DumpError(err);
	}
	
	//m_fInfo.Write(strOutput,strOutput.GetLength());
    MessageBox ("Export successful");
	m_fInfo.Write(strOutput,strOutput.GetLength());
    m_fInfo.Close();
    
}

void CEditData::swap(double &x, double &y)
{
	 double tmp;
	 tmp=x;
     x=y;
     y=tmp;
}

void CEditData::selection_sort(double A[], int n)
{
	int m;
	for(int i=1;i<n;i++)
	{
		m=i;
		for(int j=i+1;j<=n;j++)
			if(A[i]<A[m]) m=j;
			if(m!=i)	swap(A[i],A[m]);
	}
}


void CEditData::OnReport() 
{
	// TODO: Add your control notification handler code here
    
	//m_sSQL = "Select * From ["+cs+"]";
    CString fname;
	CFile m_fInfo;
    double heso_a,heso_b;
    char* temp;
    //CString sz;

 	_variant_t Index,m_vBook,IndexNext,varV,varX,var;
    _variant_t temps[NUMCHANNEL];

	double array[5000];
	
    Index.vt = VT_I2;
    IndexNext.vt = VT_I2;
	CString strOutput =" ";
	int row=0,field;
    
    temp = new char[100];
    GetSystemDirectory(temp, 100);
	
    fname = temp;
    CString fstring = m_sBrowse;
	fstring.TrimRight(".mdb");
	 
	fname = fstring + "_R.las";
   
    //m_fInfo.SetFilePath(fname);
	
		 
		 //truong hop bknext
		 //heso_a = 0.927852667;
		 //heso_b = 4875;

    UpdateData(TRUE);
	heso_a = m_dA;
	heso_b = m_dB;
	m_fInfo.SetFilePath(m_sName);
    CString dz;
	m_cStep.GetLBText(m_cStep.GetCurSel(),dz);

    CString cs;
	CString sc;
    CString sz;
	CString sFrom2;
    CString sFrom1;
    CString sFrom3;
	CString sf;
	CString st;
	CString sr;
    //CString dx;
	CString m_sql;
    CString strt;
    CString stop;

    int label ;
    CString sn;

    m_cMessage.SetWindowText("Please waitting for minute....");

	int j = 1;
	//OnSelendokComboDirection() ;
	CString dr;
	m_cDirection.GetLBText(m_cDirection.GetCurSel(),dr);
	
	if(!dr.GetLength()) return;
	//m_sTemp = cs;

   
    m_cTable.GetLBText(m_cTable.GetCurSel(),cs);

    // selected item CListBox
	CListBox *team = (CListBox*) GetDlgItem(IDC_LIST1);

	if(team->GetSelCount()>0){
			int selected[NUMCHANNEL];
	        CString buffer;
            int numSelected = team->GetSelItems(NUMCHANNEL,selected);
           
	        team->GetText(selected[0],buffer);	
            sf += ""+buffer+"" + ".M\t                      ";
		    sf +=  ":" ;
            sf += "\r\n";
            sFrom2= sFrom2 + "["+buffer+"],";
            st = ""+buffer+"" + "       "; 

			for(int i=1;i<numSelected;i++){
			
				team->GetText(selected[i],buffer);
			
                 sFrom2= sFrom2 + "["+buffer+"],";
				  
				 if(mbMSC){

					  if (buffer == "MITROT") sf = sf + ""+buffer+"" +".DEG"+"\t              "  ;
					  else if (buffer == "TE") sf = sf + ""+buffer+"" +".DEGC"+"\t                      "  ;
					  else if (buffer == "TI") sf = sf + ""+buffer+"" +".DEGC"+"\t                      "  ;
					  else if (buffer == "MAXDIA") sf = sf + ""+buffer+"" +".MM"+"\t              "  ;
					  else if (buffer == "MINDIA") sf = sf + ""+buffer+"" +".MM"+"\t              "  ;
					  else if (buffer == "AVEDIA") sf = sf + ""+buffer+"" +".MM"+"\t              "  ;
					  else  sf = sf + ""+buffer+"" +".MM"+"\t                      "  ;
				 }
				 else sf += ""+buffer+"" +            "\t                      ";
				  sf +=  ":" ;
        
				  sf += "\r\n";
				  st += ""+buffer+""; 
     
				  st += "       ";
			}
	}
			
	sFrom2.TrimRight(',');
    
	
	if(!cs.GetLength()) return;
	
	//m_sCaption = m_sBrowse+":"+cs;

	sFrom1 = "Select ";
    sFrom3 =" From ["+cs+"]";
    m_sFrom = sFrom1 + sFrom2  + sFrom3;
	
	
	m_sWhere = "";
	m_sLike = "";
	m_sQuery = "";
   
 
   
	//m_sSQL = m_sFrom + m_sWhere + m_sLike;
	//UpdateData(FALSE);

	m_sql = m_sFrom + m_sWhere; //+ m_sOrder; 
	UpdateData(FALSE);
    //ListViewData(m_sql);

    //if (!m_fInfo.Open(fname, CFile::modeCreate | CFile::modeWrite))
	if (!m_fInfo.Open(m_sName, CFile::modeCreate | CFile::modeWrite))
        MessageBox ("Unsuccessful");

    
    InitRecordset(m_sql);

    try	{	
	   
		//strOutput = sf ;
	    _variant_t var1,var2,var3 ;
		
		double min = 0,max;
		//double t,w ;
		CString s0,s1;
		int ns = 0;
		double val1,val2;
        int l=0;
		CString str;
		 _variant_t mstop;
		 double array[4000];
		 int id=0;
		
		
		// file las

    	strOutput = "~Version Information";
    	strOutput += "\r\n";
        strOutput += "VERS.		2.0:";
        
		strOutput += "\r\n";
        strOutput += "WRAP.		NO:";

        strOutput += "\r\n";
        strOutput += "~Well Information Block";
        
		
		///////////////////////////////////////////////
	
		
		for(int i=0;i< (int)m_rRs->GetFields()->GetCount();i++){
			sr = (LPCTSTR)m_rRs->GetFields()->GetItem((long)i)->GetName();
			if(sr == "DEPTH") {
				IndexNext.iVal = i;
				break;
			}
		}
		var1 = m_rRs->GetFields()->GetItem(IndexNext)->Value;
		var1.fltVal=var1.fltVal*heso_a+heso_b;
		double k =  var1.fltVal;
		
		mstop.fltVal=var1.fltVal;
		m_rRs->MoveNext();
		
        var2 = m_rRs->GetFields()->GetItem(IndexNext)->Value;
		var2.fltVal=var2.fltVal*heso_a+heso_b;
			
		
		while(VARIANT_TRUE != m_rRs->GetAdoEOF()){
			
		   	var2 = m_rRs->GetFields()->GetItem(IndexNext)->Value;
			var2.fltVal=var2.fltVal*heso_a+heso_b;
			
			m_rRs->MoveNext();
			if(m_rRs->GetAdoEOF()==VARIANT_TRUE)
				break;	
			var3 =  m_rRs->GetFields()->GetItem(IndexNext)->Value;
			var3.fltVal=var3.fltVal*heso_a+heso_b;

			if(dr == "Down"){
				if((var2.fltVal<var3.fltVal)&&(var3.fltVal>mstop.fltVal))
						mstop.fltVal=var3.fltVal;	
			}else if(dr=="Up"){
				if((var2.fltVal>var3.fltVal)&&(var3.fltVal<mstop.fltVal))
						mstop.fltVal=var3.fltVal;
			}
		}


		if(var1.fltVal > var2.fltVal) label = 1;
		else	label = 0;
			
        if(dz == "0.01") {
				strt.Format("%.2f",var1.fltVal);
				stop.Format("%.2f",mstop.fltVal);
		}else if(dz == "0.1"){
		    	strt.Format("%.1f",var1.fltVal);
				stop.Format("%.1f",mstop.fltVal);
		}else if(dz == "1"){
				strt.Format("%.0f",var1.fltVal);
				stop.Format("%.0f",mstop.fltVal);
		}


        strOutput += "\r\n";
		strOutput += "STRT.M            ";
        strOutput += strt;
	    strOutput += ":";
		strOutput += "\r\n";
		strOutput += "STOP.M            ";
	
        strOutput += stop;
        strOutput += ":";

		strOutput += "\r\n";

		strOutput += "STEP.M            ";	
	
		if(dr=="Down"){
            strOutput +=  "+";
        	strOutput +=  dz;


		}else{
            strOutput +=  "-";
        	strOutput +=  dz;
		}
        strOutput += ":";

        strOutput += "\r\n";
        strOutput += "~Curve Information Block";
        strOutput += "\r\n";
        strOutput += sf;
     
        //strOutput += "~Parameter Information Block";
        //strOutput += "\r\n";
        strOutput += "~A " ;
        //strOutput += "   ";
        
        //strOutput +=  st ;

        strOutput += "\r\n";
		
       
		m_rRs->MoveFirst();
	   
		sc = (LPCTSTR)m_rRs->GetFields()->GetItem((long)l)->GetName();    						
		if(sc=="DEPTH") { 
				varV = m_rRs->GetFields()->GetItem(IndexNext)->Value;
				varV.fltVal=varV.fltVal*heso_a+heso_b;
				if(dz == "0.01") 
						str.Format("%.2f",varV.fltVal);														
				else if(dz == "0.1")
						str.Format("%.1f",varV.fltVal);
				else if(dz == "1")
						str.Format("%.0f",varV.fltVal);
				else if(dz== "None" )
						str.Format("%.3f",varV.fltVal);														
		}
		strOutput  += "   " + str; 
 		for(l=1;l<(int)m_rRs->GetFields()->GetCount();l++){							
	         _variant_t var = m_rRs->GetFields()->GetItem((long)l)->Value;
		     str.Format("%.3f",var.fltVal);
	         strOutput += "   " + str; 
		}
      
		strOutput += "\r\n";
		
	     
       /////////////////////////////////////////////////////////////////////////////////
        	
	  	while(VARIANT_TRUE != m_rRs->GetAdoEOF()){
	
				CString sw;
				CString sh;
                double varx;

				if(dz=="0.01"){

						var = m_rRs->GetFields()->GetItem(IndexNext)->Value;
						var.fltVal=var.fltVal*heso_a+heso_b;
						val1 = var.fltVal;

						for(int i=0;i<(int)m_rRs->GetFields()->GetCount();i++){	
							Index.iVal = i;
							varV = m_rRs->GetFields()->GetItem(Index)->Value;
							temps[i] = varV;
						}
						
						m_rRs->MoveNext();
						
						varX =  m_rRs->GetFields()->GetItem(IndexNext)->Value;
						varX.fltVal=varX.fltVal*heso_a+heso_b;
										
						val2 = varX.fltVal;

				}
				else if (dz=="0.1"){
						var = m_rRs->GetFields()->GetItem(IndexNext)->Value;
						var.fltVal=var.fltVal*heso_a+heso_b;
						sw.Format("%.1f",var.fltVal);
							
						val1 = atof(sw);

						for(int i=0;i<(int)m_rRs->GetFields()->GetCount();i++){	
							Index.iVal = i;
							varV = m_rRs->GetFields()->GetItem(Index)->Value;
							temps[i] = varV;
						}

						m_rRs->MoveNext();	
						
						varX =  m_rRs->GetFields()->GetItem(IndexNext)->Value;
						varX.fltVal=varX.fltVal*heso_a+heso_b;
				
						sh.Format("%.1f",varX.fltVal);
						
						val2 = atof(sh);
		
				}
				else if (dz=="1"){
						var = m_rRs->GetFields()->GetItem(IndexNext)->Value;
						var.fltVal=var.fltVal*heso_a+heso_b;
                    	sw.Format("%.0f",var.fltVal);
							
						val1 = atof(sw);
														

						for(int i=0;i<(int)m_rRs->GetFields()->GetCount();i++){	
							Index.iVal = i;
							varV = m_rRs->GetFields()->GetItem(Index)->Value;
							temps[i] = varV;
						}

						m_rRs->MoveNext();	
						
						varX =  m_rRs->GetFields()->GetItem(IndexNext)->Value;
						varX.fltVal=varX.fltVal*heso_a+heso_b;
												
						sh.Format("%.0f",varX.fltVal);
						
						val2 = atof(sh);
	
				}

				if(label==0){
					if(dr=="Down"){
						if((val2 > val1)  && (val2 > min )){	

							for(int i=0;i<(int)m_rRs->GetFields()->GetCount();i++){	
								sc = (LPCTSTR)m_rRs->GetFields()->GetItem((long)i)->GetName();
								
								if(sc=="DEPTH") {
									varV = m_rRs->GetFields()->GetItem(IndexNext)->Value;
									varV.fltVal=varV.fltVal*heso_a+heso_b;

									if(dz == "0.01") {
										sz.Format("%.2f",varV.fltVal);
									/*	double val=(val2-val1)*100;
										for(int i=1;i<val;i=i++)
										{
											CString st;
											double varS;
											varS=val1+i/100;
											st.Format("%.2f",varS);
											strOutput +=  "*       *" + st; 
											strOutput += "\r\n";
										}*/
									}else if (dz == "0.1"){
										sz.Format("%.1f",varV.fltVal);
									/*	double val=(val2-val1)*10;
										for(int i=1;i<val;i=i++)
										{
											CString st;
											double varS;
											varS=val1+i/10;
											st.Format("%.1f",varS);
											strOutput +=  "*       *" + st; 
											strOutput += "\r\n";
										}*/
									}else if(dz == "1"){
										sz.Format("%.0f",varV.fltVal);
										/*double val=val2-val1;
										for(int i=1;i<val;i=i++)
										{
											CString st;
											double varS;
											varS=val1+i;
											st.Format("%.0f",varS);
											strOutput +=  "*       *" + st; 
											strOutput += "\r\n";
										}*/
									}else if (dz== "None"){
										sz.Format("%.3f",varV.fltVal);
									}
									
								}else {
			
									Index.iVal = i;
									varV = m_rRs->GetFields()->GetItem(Index)->Value;
									
			
									if(varV.vt == VT_BSTR){
										char ss[256];
										//m_rRs->Fields->GetItem(Index)->GetDataFormat().GetFont

										WideCharToMultiByte(CP_ACP,0,varV.bstrVal,-1,ss,256,NULL,NULL);
									}
									else if(varV.vt == VT_I4){
										sz.Format("%ld",varV.lVal);
									}
									else if(varV.vt == VT_I2){
										sz.Format("%d",varV.lVal);
									}
									else if(varV.vt == VT_DATE){
										COleDateTime time = varV.date;
										sz = time.Format("%d/%m/%Y");	
									}
									else if(varV.vt == VT_CY){
										sz.Format("%g",varV.cyVal.int64/10000.0);
									}
									else if(varV.vt == (VT_ARRAY|VT_UI1)){
										BYTE HUGEP *buf = NULL;
										_variant_t varI;
										long lsize = 0;				
										lsize = m_rRs->Fields->GetItem(Index)->GetActualSize();
								
										if(lsize){
											VariantInit(&varI);
											varI = m_rRs->Fields->GetItem(Index)->GetChunk(lsize);
											sz.Format("Picture%d",j);
											j++;					
										}
									}
									else if(varV.vt == VT_R4){
										sz.Format("%.3f",varV.fltVal);
									}
									else if(varV.vt == VT_R8){
										sz.Format("%.3f",varV.dblVal);
									}
									else {
										sz.Format("%x",varV.vt);
									}
								}
								
							    strOutput +=  "   " + sz; 
							 
							}	
								strOutput += "\r\n";
						
							// m_fInfo.Write(strOutput,strOutput.GetLength());  
			
						}
						else {

							if (val1>min){
								min = val1;
								continue;
							}
							else if(val1<min){
								continue;
							}
						}
					}else if(dr=="Up"){
						if(val1>val2){
							array[id] = val2;
							id++;
						}else {
								continue;		
						}
					}
					
				}else if(label==1){
					max = k;
					if(dr=="Up"){
						if(val1 > val2 && val2<max){
					
							for(int i=0;i<(int)m_rRs->GetFields()->GetCount();i++){	
							sc = (LPCTSTR)m_rRs->GetFields()->GetItem((long)i)->GetName();
								if(sc=="DEPTH") {
									varV = m_rRs->GetFields()->GetItem(IndexNext)->Value;
									varV.fltVal=varV.fltVal*heso_a+heso_b;

									if(dz == "0.01") 
											sz.Format("%.2f",varV.fltVal);
									else if(dz == "0.1")	
											sz.Format("%.1f",varV.fltVal);
									else if(dz == "1")
											sz.Format("%.0f",varV.fltVal);
									else if(dz== "None" )
											sz.Format("%.3f",varV.fltVal);
								}else{
			
									Index.iVal = i;
									varV = m_rRs->GetFields()->GetItem(Index)->Value;
								
			
									if(varV.vt == VT_BSTR){
										char ss[256];
										//m_rRs->Fields->GetItem(Index)->GetDataFormat().GetFont

										WideCharToMultiByte(CP_ACP,0,varV.bstrVal,-1,ss,256,NULL,NULL);
									}
									else if(varV.vt == VT_I4){
										sz.Format("%ld",varV.lVal);
									}
									else if(varV.vt == VT_I2){
										sz.Format("%d",varV.lVal);
									}
									else if(varV.vt == VT_DATE){
										COleDateTime time = varV.date;
										sz = time.Format("%d/%m/%Y");	
									}
									else if(varV.vt == VT_CY){
										sz.Format("%g",varV.cyVal.int64/10000.0);
									}
									else if(varV.vt == (VT_ARRAY|VT_UI1)){
										BYTE HUGEP *buf = NULL;
										_variant_t varI;
										long lsize = 0;				
										lsize = m_rRs->Fields->GetItem(Index)->GetActualSize();
								
										if(lsize){
											VariantInit(&varI);
											varI = m_rRs->Fields->GetItem(Index)->GetChunk(lsize);
											sz.Format("Picture%d",j);
											j++;					
										}
									}
									else if(varV.vt == VT_R4){
										sz.Format("%.3f",varV.fltVal);
									}
									else if(varV.vt == VT_R8){
										sz.Format("%.3f",varV.dblVal);
									}
									else {
										sz.Format("%x",varV.vt);
									}
								}
								strOutput +=  "   " + sz;  
							}
							strOutput += "\r\n";
						
				   	
						}else {
							if (val1<max){
								max = val1;
								continue;
							}else if(val1>max){
								continue;
							}
						}	
					}else if(dr=="Down"){
						if(val1 < val2){
							array[id] = val2;
							id++;
						
						}else {
							continue;			
					}
				}
		

			}// end else (label=1)  
		}

		if(label==0){
			if(dr=="Up"){
				CString st;
				selection_sort(array,id);
				for(int i=0;i<id;i++)
				{
					if(dz == "0.01") {
						st.Format("%.2f",array[i]);
					}else if(dz == "0.1"){	
						st.Format("%.1f",array[i]);
					}else if(dz == "1"){
						st.Format("%.0f",array[i]);
					}else if(dz== "None"){
						st.Format("%.3f",array[i]);
					}
					
					strOutput +=  "   " + st;  
					strOutput += "\r\n";
				}
			}

		}/*else{
			if(dr=="Down"){
			

			}
		}*/


        
	}
	 catch(_com_error &err)
	{
		DumpError(err);	
	}

	field = (int)m_rRs->GetFields()->GetCount();
	
    //m_fInfo.Write(strOutput,strOutput.GetLength());
	//SortFieldFollowUp(arrays,row,field);
    //Sort arrays 

/*	if(label==0){
		if(dr=="Up"){
			for(int i=0;i<row;i=i+field)
		    {
				for(int j=i+field;j<row;j=j+field)
				{
					CString s1,s2;
					_variant_t var1 = arrays[i];
				    _variant_t var2 = arrays[j];
				
					if(var1.fltVal<var2.dblVal){
						for(int k=0;k<field;k++){
 							_variant_t tam; 
							tam = arrays[i+k];
							arrays[i+k] = arrays[j+k];
							arrays[j+k] = tam;
							//Swap(arrays[i+k],arrays[j+k]);
						}
					}	
				}
			}

             // phan them moi

            for(int j=0;j<row;j=j+field)
			{
				_variant_t var,vars;
				CString sf,sg;
				var = arrays[j];
				double h = var.fltVal;
				vars = arrays[j+field];	 		
				double g = vars.fltVal;
				if(h==g){
					j=j+field;
				}else{
				
					for(int k=0;k<field;k++)
					{
						_variant_t vas;
						vas = arrays[j+k];
						CString sv;
					
						if(k==0){
							
							vas = arrays[j+k];
					
					     	if(dz == "0.01") 
								sv.Format("%.2f",vas.fltVal);		
					    	else if (dz == "0.1")		
								sv.Format("%.1f",vas.fltVal);		
					    	else if(dz == "1")	
								sv.Format("%.0f",vas.fltVal);		
					    	else if (dz== "None" )
						
								sv.Format("%.3f",vas.fltVal);
						}else{
		
							if(vas.vt == VT_BSTR){
								char ss[256];
						    	//m_rRs->Fields->GetItem(Index)->GetDataFormat().GetFont

							   	WideCharToMultiByte(CP_ACP,0,vas.bstrVal,-1,ss,256,NULL,NULL);
								}
								else if(vas.vt == VT_I4){
							     	sv.Format("%ld",vas.lVal);
								}
								else if(vas.vt == VT_I2){
									sv.Format("%d",vas.lVal);
								}
								else if(vas.vt == VT_DATE){
									COleDateTime time = vas.date;
									sv = time.Format("%d/%m/%Y");	
								}
								else if(vas.vt == VT_CY){
									sv.Format("%g",vas.cyVal.int64/10000.0);
								}
								else if(vas.vt == (VT_ARRAY|VT_UI1)){
									BYTE HUGEP *buf = NULL;
									_variant_t varI;
									long lsize = 0;				
									lsize = m_rRs->Fields->GetItem(Index)->GetActualSize();
								
									if(lsize){
										VariantInit(&varI);
										varI = m_rRs->Fields->GetItem(Index)->GetChunk(lsize);
										sv.Format("Picture%d",j);
										j++;					
									}
								}
								else if(vas.vt == VT_R4){
									sv.Format("%.3f",vas.fltVal);
								}
								else if(vas.vt == VT_R8){
									sv.Format("%.3f",vas.dblVal);
								}
								else {
									sv.Format("%x",vas.vt);
								}
							}
						strOutput +=  sv + "  ";
					}
					strOutput += "\r\n";
			}		
				//het 

		}
	}else if(label==1){
		if(dr=="Down"){
			for(int i=0;i<row;i=i+field)
			{
				for(int j=i+field;j<row;j=j+field)
				{
					CString s1,s2;
					_variant_t var1 = arrays[i];
					_variant_t var2 = arrays[j];
					if(var1.fltVal>var2.fltVal){
						for(int k=0;k<field;k++){
 							_variant_t tam; 
							tam = arrays[i+k];
  							arrays[i+k] = arrays[j+k];
							arrays[j+k] = tam;
						}
					}
					
				}
			}

            // phan them moi

			for(int j=0;j<row;j=j+field)
			{
				for(int k=0;k<field;k++)
				{
						_variant_t vas;
						CString sv;
						vas = arrays[j+k];
						
						if(k==0){
							if(dz == "0.01") 
								sv.Format("%.2f",vas.fltVal);
							else if (dz == "0.1")
								sv.Format("%.1f",vas.fltVal);
							else if(dz == "1")	
								sv.Format("%.0f",vas.fltVal);
							else if (dz== "None" )
								sv.Format("%.3f",vas.fltVal);
						}else{

							if(vas.vt == VT_BSTR){
								char ss[256];
								//m_rRs->Fields->GetItem(Index)->GetDataFormat().GetFont

								WideCharToMultiByte(CP_ACP,0,vas.bstrVal,-1,ss,256,NULL,NULL);
							}
							else if(vas.vt == VT_I4){
								sv.Format("%ld",vas.lVal);
							}
							else if(vas.vt == VT_I2){
								sv.Format("%d",vas.lVal);
							}
							else if(vas.vt == VT_DATE){
								COleDateTime time = vas.date;
								sv = time.Format("%d/%m/%Y");	
							}
							else if(vas.vt == VT_CY){
								sv.Format("%g",vas.cyVal.int64/10000.0);
							}
							else if(vas.vt == (VT_ARRAY|VT_UI1)){
								BYTE HUGEP *buf = NULL;
								_variant_t varI;
								long lsize = 0;				
								lsize = m_rRs->Fields->GetItem(Index)->GetActualSize();
								
								if(lsize){
									VariantInit(&varI);
									varI = m_rRs->Fields->GetItem(Index)->GetChunk(lsize);
									sv.Format("Picture%d",j);
									j++;					
								}
							}
							else if(vas.vt == VT_R4){
								sv.Format("%.3f",vas.fltVal);
							}
							else if(vas.vt == VT_R8){
								sv.Format("%.3f",vas.dblVal);
							}
							else {
								sv.Format("%x",vas.vt);
							}
						}
						strOutput +=  sv + "  ";
					}
					strOutput += "\r\n";
				}		

			}

		}
	}

     
 /*  if(label==0)
	{
		if(dr=="Up")
		{
			for(int j=0;j<row;j=j+field)
			{
				_variant_t var,vars;
				CString sf,sg;
				double h,g;
				var = arrays[j];
				if(var.vt == VT_R4){
					sf.Format("%.2f",var.fltVal);
				}	
				h = atof(sf);
				vars = arrays[j+field];
				if(vars.vt == VT_R4){
					  sg.Format("%.2f",vars.fltVal);
				}	
				g = atof(sg);

				if(h==g){
					j=j+field;
				}else {
				
					for(int k=0;k<field;k++)
					{
						_variant_t vas;
						vas = arrays[j+k];
						CString sv;
					
						if(k==0){
							
							vas = arrays[j+k];
					
							if(dz == "0.01") {
								if(vas.vt == VT_R4){
								sv.Format("%.2f",vas.fltVal);
							}
							else if(vas.vt == VT_R8){
								sv.Format("%.2f",vas.dblVal);
							}	
						}else if (dz == "0.1"){
							if(vas.vt == VT_R4){
								sv.Format("%.1f",vas.fltVal);
							}
							else if(vas.vt == VT_R8){
								sv.Format("%.1f",vas.dblVal);
							}
						}else if(dz == "1"){	
							if(vas.vt == VT_R4){
								sv.Format("%.0f",vas.fltVal);
							}
							else if(vas.vt == VT_R8){
								sv.Format("%.0f",vas.dblVal);
							}	
						}else if (dz== "None" ){
							if(vas.vt == VT_R4){
								sv.Format("%.3f",vas.fltVal);
							}
							else if(vas.vt == VT_R8){
								sv.Format("%.3f",vas.dblVal);
							}
						}

						}else{
		
							if(vas.vt == VT_BSTR){
								char ss[256];
						    	//m_rRs->Fields->GetItem(Index)->GetDataFormat().GetFont

							   	WideCharToMultiByte(CP_ACP,0,vas.bstrVal,-1,ss,256,NULL,NULL);
								}
								else if(vas.vt == VT_I4){
							     	sv.Format("%ld",vas.lVal);
								}
								else if(vas.vt == VT_I2){
									sv.Format("%d",vas.lVal);
								}
								else if(vas.vt == VT_DATE){
									COleDateTime time = vas.date;
									sv = time.Format("%d/%m/%Y");	
								}
								else if(vas.vt == VT_CY){
									sv.Format("%g",vas.cyVal.int64/10000.0);
								}
								else if(vas.vt == (VT_ARRAY|VT_UI1)){
									BYTE HUGEP *buf = NULL;
									_variant_t varI;
									long lsize = 0;				
									lsize = m_rRs->Fields->GetItem(Index)->GetActualSize();
								
									if(lsize){
										VariantInit(&varI);
										varI = m_rRs->Fields->GetItem(Index)->GetChunk(lsize);
										sv.Format("Picture%d",j);
										j++;					
									}
								}
								else if(vas.vt == VT_R4){
									sv.Format("%.6f",vas.fltVal);
								}
								else if(vas.vt == VT_R8){
									sv.Format("%.5f",vas.dblVal);
								}
								else {
									sv.Format("%x",vas.vt);
								}
							}
						strOutput +=  sv + "\t";
					}
					strOutput += "\r\n";
			}		
		}
	}else if(label==1){
		if(dr=="Down")
		{
			for(int j=0;j<row;j=j+field)
			{
				for(int k=0;k<field;k++)
				{
						_variant_t vas;
						CString sv;
						vas = arrays[j+k];
						
						if(k==0){
							if(dz == "0.01") {
								if(vas.vt == VT_R4){
									sv.Format("%.2f",vas.fltVal);
								}
								else if(vas.vt == VT_R8){
									sv.Format("%.2f",vas.dblVal);
								}
							}else if (dz == "0.1"){
								if(vas.vt == VT_R4){
									sv.Format("%.1f",vas.fltVal);
								}
								else if(vas.vt == VT_R8){
									sv.Format("%.1f",vas.dblVal);
								}
							}else if(dz == "1"){	
								if(vas.vt == VT_R4){
									sv.Format("%.0f",vas.fltVal);
								}
								else if(vas.vt == VT_R8){
									sv.Format("%.0f",vas.dblVal);
								}	
							}else if (dz== "None" ){
								if(vas.vt == VT_R4){
									sv.Format("%.3f",vas.fltVal);
								}
								else if(vas.vt == VT_R8){
									sv.Format("%.3f",vas.dblVal);
								}		
							}
						}else{

							if(vas.vt == VT_BSTR){
								char ss[256];
								//m_rRs->Fields->GetItem(Index)->GetDataFormat().GetFont

								WideCharToMultiByte(CP_ACP,0,vas.bstrVal,-1,ss,256,NULL,NULL);
							}
							else if(vas.vt == VT_I4){
								sv.Format("%ld",vas.lVal);
							}
							else if(vas.vt == VT_I2){
								sv.Format("%d",vas.lVal);
							}
							else if(vas.vt == VT_DATE){
								COleDateTime time = vas.date;
								sv = time.Format("%d/%m/%Y");	
							}
							else if(vas.vt == VT_CY){
								sv.Format("%g",vas.cyVal.int64/10000.0);
							}
							else if(vas.vt == (VT_ARRAY|VT_UI1)){
								BYTE HUGEP *buf = NULL;
								_variant_t varI;
								long lsize = 0;				
								lsize = m_rRs->Fields->GetItem(Index)->GetActualSize();
								
								if(lsize){
									VariantInit(&varI);
									varI = m_rRs->Fields->GetItem(Index)->GetChunk(lsize);
									sv.Format("Picture%d",j);
									j++;					
								}
							}
							else if(vas.vt == VT_R4){
								sv.Format("%.6f",vas.fltVal);
							}
							else if(vas.vt == VT_R8){
								sv.Format("%.5f",vas.dblVal);
							}
							else {
								sv.Format("%x",vas.vt);
							}
						}
						strOutput +=  sv + "\t";
					}
					strOutput += "\r\n";
				}		

			}
		}
	}  */

    m_fInfo.Write(strOutput,strOutput.GetLength());
	CString f= "File " + m_sName + " created";
    m_cMessage.SetWindowText("");
    MessageBox (f,NULL,MB_OK);

    
    m_fInfo.Close();

}


void CEditData::OnSelendokComboDirection() 
{
	// TODO: Add your control notification handler code here

	CString dz;
	m_cDirection.GetLBText(m_cDirection.GetCurSel(),dz);
	
	if(!dz.GetLength()) return;
	//m_sTemp = cs;

	//m_sCaption = m_sOpenDB+":"+cs;


    CString cs;
	CString sFrom2;
    CString sFrom1;
    CString sFrom3;

    m_cTable.GetLBText(m_cTable.GetCurSel(),cs);

    // selected item CListBox
	CListBox *team = (CListBox*) GetDlgItem(IDC_LIST1);
	if(team->GetSelCount()>0){
		//int selected[11];
        //int numSelected = team->GetSelItems(11,selected);
		int selected[NUMCHANNEL];
        int numSelected = team->GetSelItems(NUMCHANNEL,selected);

		for(int i=0;i<numSelected;i++){
			CString buffer;
			team->GetText(selected[i],buffer);
            sFrom2= sFrom2 + "["+buffer+"],";
	           
		}
		
	}
	
			
	
	sFrom2.TrimRight(',');
	if(!cs.GetLength()) return;
	
	//m_sCaption = m_sBrowse+":"+cs;

	sFrom1 = "Select ";
    sFrom3 =" From ["+cs+"]";
    m_sFrom  = sFrom1 + sFrom2 + sFrom3;
	
	
	m_sWhere = "";
	m_sLike = "";
	m_sQuery = "";


   
	//m_sSQL = m_sFrom + m_sWhere + m_sLike;
	//UpdateData(FALSE);


	m_sSQL = m_sFrom + m_sWhere ;//+  m_sOrder ;
	UpdateData(FALSE);


	OnSqlDb(m_sSQL) ;

	//if(!m_iFlag) OnSqlDb() ;

}

void CEditData::OnSelendokComboStep() 
{
	// TODO: Add your control notification handler code here

	
    CString dz;
    CString cs;
    CString sFrom1;
	CString sFrom2; 
    CString sFrom3;
   _RecordsetPtr m_rs;


	//OnSelendokComboStep() ;
	m_cStep.GetLBText(m_cStep.GetCurSel(),dz);
	
	if(!dz.GetLength()) return;
	//m_sTemp = cs;
     CString dx;
	//OnSelendokComboDirection() ;
	m_cDirection.GetLBText(m_cDirection.GetCurSel(),dx);
	

    if(dz == "0.01") {
			sFrom1 = "select FORMAT(ROUND([DEPTH],2)),";
		
	}else if (dz == "0.1"){
			sFrom1 = "select FORMAT(ROUND([DEPTH],1)),";
	       

	}else if(dz == "1"){
			sFrom1 = "select FORMAT(ROUND([DEPTH])),";
	      
	}else if(dz == "None"){
			sFrom1 = "select [DEPTH],";
	       
  
	}
	//m_sCaption = m_sOpenDB+":"+cs;

    m_cTable.GetLBText(m_cTable.GetCurSel(),cs);

    CListBox *team = (CListBox*) GetDlgItem(IDC_LIST1);
	if(team->GetSelCount()>0){
		int selected[NUMCHANNEL];
        int numSelected = team->GetSelItems(NUMCHANNEL,selected);
		//int selected[100];
        //int numSelected = team->GetSelItems(100,selected);

		for(int i=0;i<numSelected;i++){
			CString buffer;
			team->GetText(selected[i],buffer);
			
            sFrom2= sFrom2 + "["+buffer+"],";
		}
		
	}

	
	sFrom2.TrimRight(',');
	sFrom2.Delete(1,8);

    // selected item CListBox
 
	  
	
	if(!cs.GetLength()) return;
	
	//m_sCaption = m_sBrowse+":"+cs;
    
    sFrom3 ="From ["+cs+"]";
    m_sFrom  = sFrom1 + sFrom2 + sFrom3 ;


	m_sLike = "";
	m_sWhere = "";

	m_sSQL = m_sFrom + m_sWhere; //+ m_sOrder;
	//UpdateData(FALSE);

	m_sSQL = m_sFrom ;//+ m_sOrder  ;
	UpdateData(FALSE);

     
	 OnSqlDb(m_sSQL) ;

}

	

void CEditData::OnSelectMSC() 
{
	// TODO: Add your control notification handler code here
	int i;
	CListBox *team = (CListBox*) GetDlgItem(IDC_LIST1);
	team->SetSel(1,1);
	if(msc60_flag){
		for(i=0; i<66;i++){
			team->SetSel(23+i,1);
		}
	}
	else {
		for(i=0; i<40;i++){
			team->SetSel(23+i,1);
		}
		for( i=60; i<66;i++){
			team->SetSel(23+i,1);
		}
	}

    
	
	
}


