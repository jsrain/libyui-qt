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

  File:	      YQLabel.cc

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#define y2log_component "qt-ui"
#include <ycp/y2log.h>

#include "utf8.h"
#include "YQUI.h"
#include "YQLabel.h"


YQLabel::YQLabel( YWidget * 		parent,
		  const string &	text,
		  bool 			isHeading,
		  bool 			isOutputField )
    : QLabel( (QWidget *) parent->widgetRep() )
    , YLabel( parent, text, isHeading, isOutputField )
{
    setWidgetRep( this );

    setTextFormat( QLabel::PlainText );
    QLabel::setText( fromUTF8( text ) );
    setIndent(0);

    if ( isHeading )
    {
	setFont( YQUI::ui()->headingFont() );
    }
    else if ( isOutputField )
    {
	setFrameStyle ( QFrame::Panel | QFrame::Sunken );
	setLineWidth(2);
	setMidLineWidth(2);
    }
		
    setMargin( YQWidgetMargin );
    setAlignment( AlignLeft | AlignTop );
}


YQLabel::~YQLabel()
{
    // NOP
}


void YQLabel::setText( const string & newText )
{
    YLabel::setText( newText );
    QLabel::setText( fromUTF8( newText ) );
}


void YQLabel::setUseBoldFont( bool useBold )
{
    setFont( useBold ?
	     YQUI::ui()->boldFont() :
	     YQUI::ui()->currentFont() );
    
    YLabel::setUseBoldFont( useBold );
}


void YQLabel::setEnabled( bool enabled )
{
    QLabel::setEnabled( enabled );
    YWidget::setEnabled( enabled );
}


int YQLabel::preferredWidth()
{
    return sizeHint().width();
}


int YQLabel::preferredHeight()
{
    return sizeHint().height();
}


void YQLabel::setSize( int newWidth, int newHeight )
{
    resize( newWidth, newHeight );
}


#include "YQLabel.moc"
