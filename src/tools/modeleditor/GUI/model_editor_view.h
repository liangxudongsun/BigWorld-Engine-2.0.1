// ModelEditorView.h : interface of the CModelEditorView class
//


#pragma once


class CModelEditorView : public CView
{
protected: // create from serialization only
	CModelEditorView();
	DECLARE_DYNCREATE(CModelEditorView)

	CRect lastRect_;

	CModelEditorDoc* GetDocument() const;

	void OnSize(UINT nType, int cx, int cy);

	void OnPaint();

	void OnKillFocus( CWnd *pNewWnd );

	virtual ~CModelEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	// Overrides
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ModelEditorView.cpp
inline CModelEditorDoc* CModelEditorView::GetDocument() const
   { return reinterpret_cast<CModelEditorDoc*>(m_pDocument); }
#endif
