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

  File:	      YQPkgRepoList.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/

// -*- c++ -*-


#ifndef YQPkgRepoList_h
#define YQPkgRepoList_h

#include "YQZypp.h"
#include "QY2ListView.h"
#include <zypp/Repository.h>
#include <zypp/Product.h>


class YQPkgRepoListItem;

typedef zypp::Repository	ZyppRepo;


/**
 * @short Display a list of zypp::Selection objects.
 **/
class YQPkgRepoList : public QY2ListView
{
    Q_OBJECT

public:

    /**
     * Constructor
     **/
    YQPkgRepoList( QWidget * parent );

    /**
     * Destructor
     **/
    virtual ~YQPkgRepoList();

    /**
     * Returns the number of enabled installation sources.
     **/
    static int countEnabledSources();


public slots:

    /**
     * Filter according to the view's rules and current selection.
     * Emits those signals:
     *    filterStart()
     *    filterMatch() for each pkg that matches the filter
     *    filterFinished()
     **/
    void filter();

    /**
     * Same as filter(), but only if this widget is currently visible.
     **/
    void filterIfVisible();

    /**
     * Add an inst source to the list.
     **/
    void addRepo( ZyppRepo src );


public:

    // Column numbers

    int nameCol()	const	{ return _nameCol;	}
    int urlCol()	const	{ return _urlCol;	}


    /**
     * Returns the currently selected item or 0 if there is none.
     **/
    YQPkgRepoListItem * selection() const;


signals:

    /**
     * Emitted when the filtering starts. Use this to clear package lists
     * etc. prior to adding new entries.
     **/
    void filterStart();

    /**
     * Emitted during filtering for each pkg that matches the filter
     * and the candidate package comes from the respective source
     **/
    void filterMatch( ZyppSel	selectable,
		      ZyppPkg	pkg );

    /**
     * Emitted during filtering for each pkg that matches the filter
     * and the candidate package does not come from the respective source
     **/
    void filterNearMatch( ZyppSel	selectable,
			  ZyppPkg	pkg );

    /**
     * Emitted when filtering is finished.
     **/
    void filterFinished();


protected slots:

    /**
     * Fill the list.
     **/
    void fillList();


private:

    //
    // Data members
    //

    int	_nameCol;
    int	_urlCol;

};



class YQPkgRepoListItem: public QY2ListViewItem
{
public:

    /**
     * Constructor
     **/
    YQPkgRepoListItem( YQPkgRepoList *parentList, ZyppRepo src );

    /**
     * Destructor
     **/
    virtual ~YQPkgRepoListItem();

    /**
     * Returns the ZYPP source this item corresponds to
     **/
    ZyppRepo zyppSrc() const { return _zyppSrc; }

    /**
     * Returns the parent list
     **/
    const YQPkgRepoList * repoList() const { return _repoList; }

    /**
     * Returns the product on a source if it has one single product
     * or 0 if there are no or multiple products.
     **/
    static ZyppProduct singleProduct( ZyppRepo src );


    // Columns

    int nameCol()	const	{ return _repoList->nameCol();	}
    int urlCol()	const 	{ return _repoList->urlCol(); 	}

protected:

    // Data members

    YQPkgRepoList *		_repoList;
    ZyppRepo			_zyppSrc;
};


#endif // ifndef YQPkgRepoList_h