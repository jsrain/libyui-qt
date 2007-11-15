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

  File:	      YQIntField.cc

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#include <qspinbox.h>

#define y2log_component "qt-ui"
#include <ycp/y2log.h>

#include "utf8.h"
#include "YQUI.h"
#include "YEvent.h"
#include "YQIntField.h"
#include "YQSignalBlocker.h"
#include "YQWidgetCaption.h"


YQIntField::YQIntField( YWidget *	parent,
			const string &	label,
			int 		minValue,
			int 		maxValue,
			int 		initialValue )
    : QVBox( (QWidget *) parent->widgetRep() )
    , YIntField( parent, label, minValue, maxValue )
{
    setWidgetRep( this );

    setSpacing( YQWidgetSpacing );
    setMargin( YQWidgetMargin );

    _caption    = new YQWidgetCaption( this, label );
    YUI_CHECK_NEW( _caption );

    _qt_spinBox = new QSpinBox( minValue, maxValue,
				1, // step
				this );
    YUI_CHECK_NEW( _qt_spinBox );
    _qt_spinBox->setValue( initialValue );

    _caption->setBuddy( _qt_spinBox );

    setValue( initialValue );

    connect( _qt_spinBox, SIGNAL( valueChanged    ( int ) ),
	     this,  	  SLOT  ( valueChangedSlot( int ) ) );
}


YQIntField::~YQIntField()
{
    // NOP
}


int
YQIntField::value()
{
    return _qt_spinBox->value();
}


void
YQIntField::setValueInternal( int newValue )
{
    YQSignalBlocker sigBlocker( _qt_spinBox );
    _qt_spinBox->setValue( newValue );
}


void
YQIntField::valueChangedSlot( int newValue )
{
    if ( notify() )
	YQUI::ui()->sendEvent( new YWidgetEvent( this, YEvent::ValueChanged ) );
    emit valueChanged( newValue );
}


void
YQIntField::setLabel( const string & newLabel )
{
    YIntField::setLabel( newLabel );
    _caption->setText( newLabel );
}


void
YQIntField::setEnabled( bool enabled )
{
    _caption->setEnabled  ( enabled );
    _qt_spinBox->setEnabled( enabled );
    YWidget::setEnabled( enabled );
}


int
YQIntField::preferredWidth()
{
    return sizeHint().width();
}


int
YQIntField::preferredHeight()
{
    return sizeHint().height();
}


void
YQIntField::setSize( int newWidth, int newHeight )
{
    resize( newWidth, newHeight );
}


bool
YQIntField::setKeyboardFocus()
{
    _qt_spinBox->setFocus();

    return true;
}


#include "YQIntField.moc"
