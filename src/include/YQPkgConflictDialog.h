/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:	      YQPkgConflictDialog.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/

// -*- c++ -*-


#ifndef YQPkgConflictDialog_h
#define YQPkgConflictDialog_h

#include <qdialog.h>


class YQPkgConflictList;
class QPushButton;


/**
 * @short Dialog that takes care of dependency checking and resolving
 * conflicts. 
 **/
class YQPkgConflictDialog: public QDialog
{
    Q_OBJECT

public:

    /**
     * Constructor. Does not pop up a dialog yet.
     * Use solveAndShowConflicts().
     **/
    YQPkgConflictDialog( QWidget * parent );

    /**
     * Destructor.
     **/
    virtual ~YQPkgConflictDialog();

    
public slots:

    /**
     * Run the package dependency solver for the current package set and open
     * the conflict dialog if there are any conflicts.
     * Returns only when the conflict dialog is closed.
     **/
    void solveAndShowConflicts();


protected:

    YQPkgConflictList *	_conflictList;
    QPushButton *	_okButton;
    QPushButton * 	_cancelButton;
};



#endif // ifndef YQPkgConflictDialog_h
