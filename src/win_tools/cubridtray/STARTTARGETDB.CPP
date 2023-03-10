/*
 * Copyright 2008 Search Solution Corporation
 * Copyright 2016 CUBRID Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */
// StartTargetDB.cpp : implementation file
//

#include "stdafx.h"
#include "cubridtray.h"
#include "StartTargetDB.h"


#include "env.h"
#include "lang.h"
//#include "MainFrm.h"

extern CLang theLang;
extern CEnv  Env;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartTargetDB dialog


CStartTargetDB::CStartTargetDB(CWnd* pParent /*=NULL*/)
	: CDialog(CStartTargetDB::IDD, pParent)
{
	cCUBRID = new CCUBRIDManage();
	//{{AFX_DATA_INIT(CStartTargetDB)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CStartTargetDB::CStartTargetDB( CCUBRIDManage* cCUBRID, CWnd* pParent /*=NULL*/)
	: CDialog(CStartTargetDB::IDD, pParent)
{
	cCUBRID = cCUBRID;
	//{{AFX_DATA_INIT(CStartTargetDB)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CStartTargetDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStartTargetDB)
	DDX_Control(pDX, ID_SU_CANCEL, m_su_cancel);
	DDX_Control(pDX, IDC_SU_LIST, m_ListDB);
	DDX_Control(pDX, ID_SU_OK, m_su_ok);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartTargetDB, CDialog)
	//{{AFX_MSG_MAP(CStartTargetDB)
	ON_BN_CLICKED(ID_SU_OK, OnSuOk)
	ON_BN_CLICKED(ID_SU_CANCEL, OnSuCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartTargetDB message handlers
#define		MAXSERVER	100

BOOL CStartTargetDB::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Windows?? Button ???? ?????? ?????? ?????? ???? ????????.
	SetWindowText(theLang.GetMessage(WN_STD_TITLE));
	m_su_ok.SetWindowText(theLang.GetMessage(WN_STD_OK));
	m_su_cancel.SetWindowText(theLang.GetMessage(WN_STD_CANCEL));

	// Master?? ????????, Master?? ?????? ?? ??????????, ??????????.
	if( cCUBRID->bCheckMaster() )
		if( !cCUBRID->bStartMaster() )
		{
			AfxMessageBox(theLang.GetMessage(MSG_MASTER_ERROR), MB_OK | MB_ICONSTOP);
			return FALSE;
		}


	// ?????? DB?? ????????. DB?? ??????, OK Buttone?? Disable????.
	DBNAMEPtr_t pStartList = cCUBRID->pReqStopDBList();
	if( !pStartList )
	{
		m_su_ok.EnableWindow(false);
		return TRUE;
	}

	DBNAMEPtr_t pCur = pStartList;
	int index = 0;

	// ListBox???? ???? ???? ???? DB?? ???? ????
	for (int i = 0; i < MAXSERVER; i++) {
		m_ListDB.InsertString( index, pCur->sName );

		if( !pCur->next ) break;
		pCur = pCur->next;
		index++;
	}

	cCUBRID->bDestoryDBList( pStartList );

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStartTargetDB::OnSuOk() 
{
	// TODO: Add your control notification handler code here
	int i, maxcount = m_ListDB.GetCount();
	CString	tmpString;
	bool bResult;
	char sSucDB[1024];
	memset( sSucDB, 0x00, sizeof( sSucDB ) );
	CString msg;

	for (i = 0; i < maxcount; i++) {
		if (m_ListDB.GetSel(i) > 0) {
			m_ListDB.GetText(i, tmpString);
			bResult = cCUBRID->bStartCUBRID((char *)LPCSTR( tmpString ) );
TRACE2( "Start DB( %s ) : %s\n", (char *)LPCSTR( tmpString ), bResult?"TRUE":"FALSE" );

			if( bResult )
			{
				if( strlen( sSucDB ) <= 0 ) sprintf( sSucDB, "%s", (char *)LPCSTR( tmpString ) );
				else                        sprintf( sSucDB, "%s, %s", sSucDB, (char *)LPCSTR( tmpString ) );
			}
		}
	}

//	msg.Empty();
//	msg.Format(theLang.GetMessage(MSG_START), sSucDB);
//	AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);

	CDialog::OnOK();
	
}

void CStartTargetDB::OnSuCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}



