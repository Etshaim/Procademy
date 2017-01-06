#include "RedBlackTree.h"
#include <stdio.h>

CRedBlackTree::CRedBlackTree()
{
	m_iNodeCount = 0;
	m_pRoot = nullptr;

	m_stNil.enColor = st_TNODE::enBLACK;
	m_stNil.pParent = nullptr;
	m_stNil.pLeft = nullptr;
	m_stNil.pRight = nullptr;
}

CRedBlackTree::~CRedBlackTree()
{
	ReleaseTree();
}

bool CRedBlackTree::InsertNode(int iData)
{
	st_TNODE *pTemp = new st_TNODE;

	pTemp->iData = iData;
	pTemp->pLeft = &m_stNil;
	pTemp->pRight = &m_stNil;
	pTemp->pParent = nullptr;
	pTemp->enColor = st_TNODE::enRED;

	if (nullptr == m_pRoot)
	{
		m_pRoot = pTemp;
		m_pRoot->enColor = st_TNODE::enBLACK;
		return true;
	}

	if (false == linkNode(m_pRoot, pTemp))
	{
		delete pTemp;

		return false;
	}

	// 밸런스 맞추기
	//balance(pTemp);
	return true;
}

bool CRedBlackTree::DeleteNode(int iData)
{
	st_TNODE *pDeleteNode = deleteNode(m_pRoot, nullptr, iData);

	if (nullptr != pDeleteNode)
	{
		delete pDeleteNode;
		return true;
	}
	else
	{
		return false;
	}
}

st_TNODE * CRedBlackTree::SearchNode(int iData)
{
	if (nullptr == m_pRoot)
	{
		return nullptr;
	}

	return findNode(m_pRoot, iData);
}

bool CRedBlackTree::ReleaseTree(void)
{
	if (nullptr != m_pRoot)
	{
		releaseNode(m_pRoot);
		return true;
	}
	else
	{
		return false;
	}
}

void CRedBlackTree::PreorderPrint(st_TNODE * pNode, int iDepth, int iLR)
{
	if (nullptr == pNode)
	{
		pNode = m_pRoot;
	}

	print(pNode, iDepth, iLR, enPreorder);
}

void CRedBlackTree::InorderPrint(st_TNODE * pNode, int iDepth, int iLR)
{
	if (nullptr == pNode)
	{
		pNode = m_pRoot;
	}

	print(pNode, iDepth, iLR, enInorder);
}

void CRedBlackTree::PostorderPrint(st_TNODE * pNode, int iDepth, int iLR)
{
	if (nullptr == pNode)
	{
		pNode = m_pRoot;
	}

	print(pNode, iDepth, iLR, enPostorder);
}


bool CRedBlackTree::linkNode(st_TNODE * pParent, st_TNODE * pChild)
{
	// 작으면 왼쪽으로
	if (pParent->iData > pChild->iData)
	{
		// 잎 노트까지 도달했다면 넣기
		if (&m_stNil == pParent->pLeft)
		{
			pParent->pLeft = pChild;
			pChild->pParent = pParent;

			balanceLink(pChild);
			m_iNodeCount++;
			return true;
		}
		else
		{
			return linkNode(pParent->pLeft, pChild);
		}
	}

	if (pParent->iData < pChild->iData)
	{
		if (&m_stNil == pParent->pRight)
		{
			pParent->pRight = pChild;
			pChild->pParent = pParent;

			balanceLink(pChild);
			m_iNodeCount++;
			return true;
		}
		else
		{
			return linkNode(pParent->pRight, pChild);
		}
	}

	return false;
}

st_TNODE * CRedBlackTree::findNode(st_TNODE * pNode, int iData)
{
	if (pNode->iData > iData)
	{
		if (nullptr == pNode->pLeft)
		{
			return nullptr;
		}

		findNode(pNode->pLeft, iData);
	}

	if (pNode->iData < iData)
	{
		if (nullptr == pNode->pRight)
		{
			return nullptr;
		}

		findNode(pNode->pRight, iData);
	}

	return pNode;
}

st_TNODE * CRedBlackTree::deleteNode(st_TNODE * pNode, st_TNODE * pParent, int iData)
{
	if (pNode->iData > iData)
	{
		if (nullptr == pNode->pLeft)
		{
			return nullptr;
		}
		else
		{
			return deleteNode(pNode->pLeft, pNode, iData);
		}
	}

	else if (pNode->iData < iData)
	{
		if (nullptr == pNode->pRight)
		{
			return nullptr;
		}
		else
		{
			return deleteNode(pNode->pRight, pNode, iData);
		}
	}
	else
	{
		// 여기까지 왔다는 것은 삭제할 노드를 찾았다는 것
		// 삭제할 노드의 상태는 3가지

		// 1.자식이 없는 경우
		if (nullptr == pNode->pLeft && nullptr == pNode->pRight)
		{
			// 부모 노드 null 처리
			if (pParent->pLeft == pNode)
			{
				pParent->pLeft = nullptr;
			}
			else
			{
				pParent->pRight = nullptr;
			}
			return pNode;
		}
		// 2. 자식이 둘 다 있는 경우
		else if (nullptr != pNode->pLeft && nullptr != pNode->pRight)
		{
			// 대타
			// 대신한 노드를 찾을 것임
			// right 자손중에 제일 작은 녀석으로
			st_TNODE *pSubstitute = pNode->pRight;

			while (nullptr != pSubstitute->pLeft)
			{
				pSubstitute = pSubstitute->pLeft;
			}

			// pSubstitute left는 null이지만 right에 자식이 있을 수도 있으므로
			pSubstitute = deleteNode(pNode, nullptr, pSubstitute->iData);

			pSubstitute->pRight = pNode->pRight;
			pSubstitute->pLeft = pNode->pLeft;

			if (pNode == m_pRoot)
			{
				m_pRoot = pSubstitute;
			}

			return pNode;
		}

		// 3. 자식이 하나만 있는 경우
		else
		{
			// 자식이 하나만 있는 경우는 그냥 대체하면 됨
			st_TNODE *pSubstitute = pNode;

			if (pNode->pLeft != nullptr)
			{
				pSubstitute = pNode->pLeft;
			}
			else
			{
				pSubstitute = pNode->pRight;
			}

			// 연결해주기
			if (pParent->pLeft == pNode)
			{
				pParent->pLeft = pSubstitute;
			}
			else
			{
				pParent->pRight = pSubstitute;
			}

			if (pNode == m_pRoot)
			{
				m_pRoot = pSubstitute;
			}

			return pNode;
		}
	}
}

void CRedBlackTree::releaseNode(st_TNODE * pNode)
{
	if (&m_stNil != pNode->pLeft)
	{
		releaseNode(pNode->pLeft);
	}

	if (&m_stNil != pNode->pRight)
	{
		releaseNode(pNode->pRight);
	}

	delete pNode;
}

void CRedBlackTree::print(st_TNODE * pParent, int iDepth, int iLR, PRINT_ODER enOrder)
{
	st_TNODE *pFirstDir = nullptr;
	st_TNODE *pSecondDir = nullptr;

	if (iLR != 0)
	{
		if (nullptr != pParent->pLeft)
			pFirstDir = pParent->pLeft;
		if (nullptr != pParent->pRight)
			pSecondDir = pParent->pRight;
	}
	else
	{
		if (nullptr != pParent->pRight)
			pFirstDir = pParent->pRight;
		if (nullptr != pParent->pLeft)
			pSecondDir = pParent->pLeft;
	}

	if (enPreorder == enOrder)
	{
		for (int iCount = 0; iCount < iDepth; iCount++)
		{
			printf("  ");
		}

		printf("%d\n", pParent->iData);
	}


	if (nullptr != pFirstDir)
	{
		print(pFirstDir, iDepth + 1, iLR, enOrder);
	}

	if (enInorder == enOrder)
	{
		for (int iCount = 0; iCount < iDepth; iCount++)
		{
			printf("  ");
		}

		printf("%d\n", pParent->iData);
	}

	if (nullptr != pSecondDir)
	{
		print(pSecondDir, iDepth + 1, iLR, enOrder);
	}

	if (enPostorder == enOrder)
	{
		for (int iCount = 0; iCount < iDepth; iCount++)
		{
			printf("  ");
		}

		printf("%d\n", pParent->iData);
	}
}

void CRedBlackTree::balanceLink(st_TNODE * pNode)
{
	if (m_pRoot == pNode || m_pRoot == pNode->pParent)
	{
		m_pRoot->enColor = st_TNODE::enBLACK;
		return;
	}


	st_TNODE *pGrandParent = pNode->pParent->pParent;
	st_TNODE *pParent = pNode->pParent;

	// parent가 검은 색일 때
	// 따로 처리해 줄 것이 없음
	if (st_TNODE::enRED != pParent->enColor)
	{
		return;
	}


	// 여기서부터는 parent가 red임

	// parent가 왼쪽일 때
	if (pParent == pGrandParent->pLeft)
	{
		// uncle은 오른쪽임

		st_TNODE *pUncle = pGrandParent->pRight;

		// ungle이 빨간 색일 때
		if (st_TNODE::enRED == pUncle->enColor)
		{
			// parent와 uncle을 black으로 변경 후
			// grandparent를 red로 변경
			pParent->enColor = st_TNODE::enBLACK;
			pUncle->enColor = st_TNODE::enBLACK;
			pGrandParent->enColor = st_TNODE::enRED;

			// red로 된 grandparent도 처리
			// 만약 grandparent가 root이면 검은색으로 바꾸고 끝
			if (pGrandParent == m_pRoot)
			{
				pGrandParent->enColor = st_TNODE::enBLACK;
				return;
			}
			return balanceLink(pGrandParent);
		}

		// uncle이 검은 색일 때
		if (st_TNODE::enBLACK == pUncle->enColor)
		{
			// pNode가 부모의 오른쪽이라면 왼쪽 회전 시켜줌
			if (pNode == pParent->pRight)
			{
				rotationLeft(pParent);

				// pNode와 pParent가 바뀜
				pNode = pParent;
				pParent = pNode->pParent;
			}

			// parent는 검은색으로 grandparent는 빨간색으로 바꿈
			pParent->enColor = st_TNODE::enBLACK;
			pGrandParent->enColor = st_TNODE::enRED;

			// grandparent를 오른쪽으로 돌림
			rotationRight(pGrandParent);
		}
	}


	// parent가 오른쪽일 때
	if (pParent == pGrandParent->pRight)
	{

		// uncle은 왼쪽
		st_TNODE *pUncle = pGrandParent->pLeft;

		// ungle이 빨간 색일 때
		if (st_TNODE::enRED == pUncle->enColor)
		{
			// parent와 uncle을 black으로 변경 후
			// grandparent를 red로 변경
			pParent->enColor = st_TNODE::enBLACK;
			pUncle->enColor = st_TNODE::enBLACK;
			pGrandParent->enColor = st_TNODE::enRED;

			// red로 된 grandparent도 처리
			return balanceLink(pGrandParent);
		}

		// uncle이 검은 색일 때
		if (st_TNODE::enBLACK == pUncle->enColor)
		{
			// pNode가 부모의 왼쪽이라면 오른쪽 회전 시켜줌
			if (pNode == pParent->pLeft)
			{
				rotationRight(pParent);

				// pNode와 pParent가 바뀜
				pNode = pParent;
				pParent = pNode->pParent;
			}

			// parent는 검은색으로 grandparent는 빨간색으로 바꿈
			pParent->enColor = st_TNODE::enBLACK;
			pGrandParent->enColor = st_TNODE::enRED;

			// grandparent를 왼쪽으로 돌림
			rotationLeft(pGrandParent);
		}
	}
}

void CRedBlackTree::balanceDelete(st_TNODE * pNode)
{
	// delete node 삭제 노드
	st_TNODE *pDelete = pNode;

	// replace node 삭제 노드의 자리를 차지할 노드
	st_TNODE *pParent = pDelete->pParent;
	st_TNODE *pReplace = nullptr;
	st_TNODE *pSibling = nullptr;



	// 만약 pDelete가 빨간색이면 따로 아무 처리해줄 필요가 없음
	if (pDelete->enColor == st_TNODE::enRED)
	{
		return;
	}

	while (1)
	{
		// 예외처리1.
		// replace가 루트이면 따로 생각할 필요없이 검은색으로 바꿔주고 종료

		if (pReplace == m_pRoot)
		{
			break;
		}

		// 예외처리2.
		// replace가 nil이면 parent를 연결 시켜줘야 함
		if (pReplace == &m_stNil)
		{
			pReplace->pParent = pParent;
		}

		// 여기서부터는 pDelete가 검은색이라는 가정
		// 검은색이기 때문에 검은 노드 수 규칙이 깨짐

		// 부모를 기준으로replace와 sibling의 위치를 정함
		if (pParent->pLeft != &m_stNil)
		{
			pReplace = pParent->pLeft;
		}
		else
		{
			pReplace = pParent->pRight;
		}

		// 왼쪽일 때
		if (pReplace == pParent->pLeft)
		{
			// 형제는 오른쪽이다
			pSibling = pParent->pRight;

			// 1. 형제가 빨간색
			if (pSibling->enColor == st_TNODE::enRED)
			{
				// 형제가 빨간색이므로 부모는 검은색

				// 부모 -> 빨강
				pParent->enColor = st_TNODE::enRED;

				// 형제 -> 검정
				pSibling->enColor = st_TNODE::enBLACK;

				// 부모를 좌회전
				rotationLeft(pParent);

				break;
			}
			// 2.형제가 검은색
			else
			{
				// 2-1 형제의 왼쪽 오른쪽이 모두 검은색
				if (pSibling->pLeft->enColor == st_TNODE::enBLACK
					&& pSibling->pRight->enColor == st_TNODE::enBLACK)
				{
					pSibling->enColor = st_TNODE::enRED;
					pParent->enColor = st_TNODE::enBLACK;
					pReplace = pParent;
					pParent = pReplace->pParent;
				}
				// 2-2 형제의 왼쪽이 빨강, 오른쪽이 검정
				else if (pSibling->pLeft->enColor == st_TNODE::enRED
					&& pSibling->pRight->enColor == st_TNODE::enBLACK)
				{
					// 형제 노드를 빨간색으로 칠하고
					pSibling->enColor = st_TNODE::enRED;

					// 형제 노드의 왼쪽 자식을 검은색으로
					pSibling->pLeft->enColor = st_TNODE::enBLACK;

					// 형제노드를 우회전
					rotationRight(pSibling);

					continue;
				}

				else
				{
					// 2-3 형제의 오른쪽이 빨강
					// 부모의 색을 형제 노드에 칠함
					// 회전을 통해 형제노드가 부모의 자리에 갈 것임
					pSibling->enColor = pParent->enColor;

					// 형제의 우측 자식을 검은색으로 칠함
					pSibling->pRight->enColor = st_TNODE::enBLACK;

					// 부모를 좌회전 시킴
					rotationLeft(pParent);

					// parent 자리에 sibling이 위치
					pReplace = pSibling->pParent;

					continue;
				}
			}
		}

		// 오른쪽일 때
		{
			// 형제는 왼쪽이다
			pSibling = pParent->pLeft;

			// 1. 형제가 빨간색
			if (pSibling->enColor == st_TNODE::enRED)
			{
				// 형제가 빨간색이므로 부모는 검은색

				// 부모 -> 빨강
				pParent->enColor = st_TNODE::enRED;

				// 형제 -> 검정
				pSibling->enColor = st_TNODE::enBLACK;

				// 부모를 우회전
				rotationRight(pParent);

				break;
			}
			// 2.형제가 검은색
			else
			{
				// 2-1 형제의 왼쪽 오른쪽이 모두 검은색
				if (pSibling->pLeft->enColor == st_TNODE::enBLACK
					&& pSibling->pRight->enColor == st_TNODE::enBLACK)
				{
					pSibling->enColor = st_TNODE::enRED;
					pReplace = pParent;
					pParent = pReplace->pParent;
				}
				// 2-2 형제의 오른쪽이 빨강, 왼쪽이 검정
				else if (pSibling->pLeft->enColor == st_TNODE::enBLACK
					&& pSibling->pRight->enColor == st_TNODE::enRED)
				{
					// 형제 노드를 빨간색으로 칠하고
					pSibling->enColor = st_TNODE::enRED;

					// 형제 노드의 오른쪽 자식을 검은색으로
					pSibling->pRight->enColor = st_TNODE::enBLACK;

					// 형제노드를 좌회전
					rotationLeft(pSibling);

					continue;
				}

				else
				{
					// 2-3 형제의 오른쪽이 빨강
					// 부모의 색을 형제 노드에 칠함
					// 회전을 통해 형제노드가 부모의 자리에 갈 것임
					pSibling->enColor = pParent->enColor;

					// 형제의 좌측 자식을 검은색으로 칠함
					pSibling->pLeft->enColor = st_TNODE::enBLACK;

					// 부모를 우회전 시킴
					rotationRight(pParent);

					// parent 자리에 sibling이 위치
					pReplace = pSibling->pParent;

					continue;
				}
			}
		}
	}

	m_pRoot->enColor = st_TNODE::enBLACK;
}

void CRedBlackTree::rotationLeft(st_TNODE * pNode)
{
	// pNode를 왼쪽으로 돌리면
	// pNode의 right가 parent가 됨
	// pNode의 right 자리는 pNode의 right의 left가 들어감

	//<	새로운 자리로 배치
	st_TNODE *pGrandParent = pNode->pParent;
	st_TNODE *pParent = pNode->pRight;
	st_TNODE *pLeft = pNode;
	st_TNODE *pLeftRight = pNode->pRight->pLeft; // left의 right

	//<	연결하기
		// pNode의 right와 pNode의 parent를 연결
		// pNode가 root가 아닌 경우
	if (nullptr != pGrandParent)
	{
		if (pGrandParent->pLeft == pNode)
		{
			pGrandParent->pLeft = pParent;
		}
		if (pGrandParent->pRight == pNode)
		{
			pGrandParent->pRight = pParent;
		}
	}
	else
	{
		// 여기에 들어왔다는 것은 pNode가 root가 루트
		m_pRoot = pParent;
	}

	pParent->pLeft = pLeft;
	pLeft->pParent = pParent;

	pLeft->pRight = pLeftRight;
	pLeftRight->pParent = pLeft;
}

void CRedBlackTree::rotationRight(st_TNODE * pNode)
{
	// pNode를 오른쪽으로 돌리면
	// pNode의 left가 parent가 됨
	// pNode의 left 자리는 pNode의 left의 right가 들어감

	//<	새로운 자리로 배치
	st_TNODE *pGrandParent = pNode->pParent;
	st_TNODE *pParent = pNode->pLeft;
	st_TNODE *pRight = pNode;
	st_TNODE *pRightLeft = pNode->pLeft->pRight;

	// pNode의 right와 pNode의 parent를 연결
	// pNode가 root가 아닌 경우
	if (nullptr != pGrandParent)
	{
		if (pGrandParent->pLeft == pNode)
		{
			pGrandParent->pLeft = pParent;
		}
		if (pGrandParent->pRight == pNode)
		{
			pGrandParent->pRight = pParent;
		}
	}
	else
	{
		// 여기에 들어왔다는 것은 pNode가 root가 루트
		m_pRoot = pParent;
	}

	pParent->pRight = pRight;
	pRight->pParent = pParent;

	pRight->pLeft = pRightLeft;
	pRightLeft->pParent = pRight;
}
