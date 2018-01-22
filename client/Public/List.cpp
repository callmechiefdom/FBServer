/*$T Common/List.cpp GC 1.140 10/10/07 10:06:27 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "List.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */

CListItem::CListItem()
{
	m_pHead = NULL;
	m_pNext = NULL;
	m_pPre = NULL;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
CListItem* CListItem::GetPrevious()
{
	return m_pPre;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
void CListItem::SetPrevious(CListItem *pItem)
{
	m_pPre = pItem;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
void CListItem::SetNext(CListItem *pItem)
{
	m_pNext = pItem;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
CListItem* CListItem::GetNext()
{
	return m_pNext;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
void CListItem::SetHeader(CListHeader *pHeader)
{
	m_pHead = pHeader;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void CListItem::DisConnect()
{
	if(m_pHead)
	{
		m_pHead->SetNum(m_pHead->GetNum() - 1);
		if(m_pHead->GetNum() == 0)
		{
			m_pHead->m_pHead = NULL;
			m_pHead->m_pEnd = NULL;
			m_pHead->m_pCurrent = NULL;
		}
		else
		{
			if(m_pHead->m_pHead == this)
			{
				m_pHead->m_pHead = m_pNext;
				m_pHead->m_pCurrent = m_pNext;
			}

			if(m_pHead->m_pEnd == this)
			{
				m_pHead->m_pEnd = m_pPre;
				m_pHead->m_pCurrent = m_pPre;
			}

			if(m_pHead->m_pCurrent == this)
			{
				m_pHead->m_pCurrent = m_pNext;
			}
		}
	}

	if(m_pNext)
        m_pNext->SetPrevious(m_pPre);
	if(m_pPre)
        m_pPre->SetNext(m_pNext);

	m_pNext = NULL;
	m_pPre = NULL;
	m_pHead = NULL;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
CListHeader* CListItem::GetHeader()
{
	return m_pHead;
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
CListHeader::CListHeader()
{
	m_uNum = 0;
	m_pEnd = NULL;
	m_pHead = NULL;
	m_pCurrent = NULL;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
CListHeader::~CListHeader()
{
	RemoveAll();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL CListHeader::Insert(CListItem *pItem)
{
	if(!m_uNum)
	{
		m_pEnd = pItem;
		m_pHead = pItem;
		m_pCurrent = pItem;
		SetNext(pItem);
		pItem->SetPrevious((CListItem *) this);
		pItem->SetNext(NULL);
		m_uNum++;
	}
	else
	{
		SetNext(pItem);
		pItem->SetPrevious((CListItem *) this);
		pItem->SetNext(m_pHead);
		m_pHead->SetPrevious(pItem);
		m_pHead = pItem;
		m_uNum++;
	}

	pItem->SetHeader(this);
	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL CListHeader::Add(CListItem *pItem)
{
	pItem->DisConnect();

	if(!m_uNum)
	{
		m_pEnd = pItem;
		m_pHead = pItem;
		m_pCurrent = pItem;
		SetNext(pItem);
		pItem->SetPrevious((CListItem *) this);
		pItem->SetNext(NULL);
		m_uNum++;
	}
	else
	{
		m_pEnd->SetNext(pItem);
		pItem->SetPrevious(m_pEnd);
		pItem->SetNext(NULL);
		m_pEnd = pItem;
		m_uNum++;
	}

	pItem->SetHeader(this);
	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
CListItem *CListHeader::MoveTo(int32 _Idx)
{
	m_pCurrent = m_pHead;
	for(int32 j = 0; j < _Idx; j++)
	{
		if(m_pCurrent->GetNext())
		{
			m_pCurrent = m_pCurrent->GetNext();
		}
		else
		{
			return NULL;
		}
	}

	return m_pCurrent;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL CListHeader::Insert_After_Current(CListItem *pItem)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	CListItem	*pNext = GetNext();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(pNext)
	{
		Insert_Before_Current(pItem);
	}
	else
	{
		Add(pItem);
	}

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL CListHeader::Insert_Before_Current(CListItem *pItem)
{
	if(!m_uNum)
	{
		m_pEnd = pItem;
		m_pHead = pItem;
		m_pCurrent = pItem;
		SetNext(pItem);
		pItem->SetPrevious((CListItem *) this);
		pItem->SetNext(NULL);
		m_uNum++;
	}
	else
	{
		if(m_pCurrent == m_pHead)
		{
			m_pHead = pItem;
		}

		pItem->SetNext(m_pCurrent);
		pItem->SetPrevious(m_pCurrent->GetPrevious());
		m_pCurrent->GetPrevious()->SetNext(pItem);
		m_pCurrent->SetPrevious(pItem);

		m_uNum++;
	}

	pItem->SetHeader(this);
	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
CListItem *CListHeader::OutChain()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	CListItem	*pReturn = m_pCurrent;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(m_pCurrent)
	{
		m_pCurrent->DisConnect();
	}

	return pReturn;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
CListItem *CListHeader::Remove()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	CListItem	*pPreviousCurrent = m_pCurrent;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(m_pCurrent)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		CListItem	*pReturn = m_pCurrent->GetNext();
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		m_pCurrent->DisConnect();
		delete pPreviousCurrent;
		return pReturn;
	}
	else
	{
		return NULL;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL CListHeader::RemoveAll()
{
	First();
	while(Remove())
	{
		;
	}

	m_uNum = 0;

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL CListHeader::OutAll()
{
	Last();
	while(m_uNum > 0)
	{
		OutChain();
	}

	m_uNum = 0;

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 CListHeader::Get(CListItem *pItem)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32		uNum = 0;
	CListItem	*temp = First();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	while(temp)
	{
		if(pItem == temp) return uNum;

		temp = Next();

		uNum++;
	}

	return -1;
}


/*
	===============================================================================================================
	===============================================================================================================
	*/

uint32 CListHeader::GetNum()
{
	return m_uNum;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
void CListHeader::SetNum(uint32 uNum)
{
	m_uNum = uNum;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
CListItem* CListHeader::First()
{
	m_pCurrent = m_pHead;
	return m_pCurrent;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
CListItem* CListHeader::Next()
{
	if(!m_pCurrent) return NULL;
	m_pCurrent = m_pCurrent->GetNext();
	return m_pCurrent;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
CListItem* CListHeader::NextCycle()
{
	if(!m_pCurrent)
		m_pCurrent = m_pHead;
	else if(m_pCurrent == m_pEnd)
		m_pCurrent = m_pHead;
	else
		m_pCurrent = m_pCurrent->GetNext();

	return m_pCurrent;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
CListItem* CListHeader::ViewFirst()
{
	return m_pHead;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
CListItem* CListHeader::ViewNext()
{
	if(!m_pCurrent) return NULL;
	return m_pCurrent->GetNext();
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
CListItem* CListHeader::Last()
{
	m_pCurrent = m_pEnd;
	return m_pCurrent;
}



/*
	===============================================================================================================
	===============================================================================================================
	*/

CListItem* CListHeader::Current()
{
	return m_pCurrent;
}

/*
	===============================================================================================================
	===============================================================================================================
	*/
void CListHeader::SetCurrent(CListItem *pItem)
{
	m_pCurrent = pItem;
}
