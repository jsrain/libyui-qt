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

  File:	      YQLogView.cc

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#include <qlabel.h>
#include <qprogressbar.h>
#include <qtimer.h>
#define y2log_component "qt-ui"
#include <ycp/y2log.h>

#include "utf8.h"
#include "YQUI.h"
#include "YQDownloadProgress.h"
#include "YQWidgetCaption.h"


YQDownloadProgress::YQDownloadProgress( YWidget *	parent,
					const string & 	label,
					const string &	filename,
					YFileSize_t	expectedSize )
    : QVBox( (QWidget *) parent->widgetRep() )
    , YDownloadProgress( parent, label, filename, expectedSize )
{
    setWidgetRep( this );
    setMargin( YQWidgetMargin );

    _caption = new YQWidgetCaption( this, label );
    YUI_CHECK_NEW( _caption );

    _qt_progressBar = new QProgressBar( this );
    YUI_CHECK_NEW( _qt_progressBar );

    _qt_progressBar->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
    _qt_progressBar->setTotalSteps( 100 ); // Using percent
    _qt_progressBar->setProgress( currentPercent() );

    _timer = new QTimer( this );

    connect( _timer, 	SIGNAL( timeout()      ),
	     this,	SLOT  ( pollFileSize() ) );

    _timer->start( 250,		// millisec
		   false );	// single shot?
}


YQDownloadProgress::~YQDownloadProgress()
{
    // NOP
}


void
YQDownloadProgress::setLabel( const string & label )
{
    _caption->setText( label );
    YDownloadProgress::setLabel( label );
}


void
YQDownloadProgress::setFilename( const string & filename )
{
    YDownloadProgress::setFilename( filename );
    _qt_progressBar->setProgress( currentPercent() );
}


void
YQDownloadProgress::setExpectedSize( YFileSize_t expectedSize )
{
    _qt_progressBar->setProgress( currentPercent() );
    YDownloadProgress::setExpectedSize( expectedSize );
}


void
YQDownloadProgress::pollFileSize()
{
    _qt_progressBar->setProgress( currentPercent() );
}


void
YQDownloadProgress::setEnabled( bool enabled )
{
    _caption->setEnabled( enabled );
    _qt_progressBar->setEnabled( enabled );
    YWidget::setEnabled( enabled );
}


int
YQDownloadProgress::preferredWidth()
{
    return sizeHint().width();
}


int
YQDownloadProgress::preferredHeight()
{
    return sizeHint().height();
}


void
YQDownloadProgress::setSize( int newWidth, int newHeight )
{
    resize( newWidth, newHeight );
}


#include "YQDownloadProgress.moc"
