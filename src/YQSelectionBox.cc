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

  File:	      YQSelectionBox.cc

  Author:     Mathias Kettner <kettner@suse.de>
  Maintainer: Stefan Hundhammer <sh@suse.de>

/-*/


#include <qstring.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qnamespace.h>
#define y2log_component "qt-ui"
#include <ycp/y2log.h>

using std::max;

#include "utf8.h"
#include "YUIQt.h"
#include "layoututils.h"
#include "YQSelectionBox.h"
#include "YQDialog.h"

#define DEFAULT_VISIBLE_LINES		5
#define SHRINKABLE_VISIBLE_LINES	2
#define MIN_WIDTH			80
#define MIN_HEIGHT			80
#define SPACING				4	// between subwidgets
#define MARGIN				4	// around the widget


YQSelectionBox::YQSelectionBox(YUIQt *yuiqt, QWidget *parent, YWidgetOpt &opt,
			       const YCPString& label)
    : QVBox(parent)
    , YSelectionBox(opt, label)
    , yuiqt(yuiqt)
{
    setWidgetRep(this);

    setSpacing( SPACING );
    setMargin( MARGIN );

    qt_label = new QLabel(fromUTF8(label->value()), this);
    qt_label->setTextFormat(QLabel::PlainText);
    qt_label->setFont(yuiqt->currentFont());

    qt_listbox = new QListBox(this);
    qt_listbox->installEventFilter( this );
    qt_listbox->setVariableHeight(false);
    qt_listbox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ));
    qt_listbox->setTopItem(0);
    qt_label->setBuddy(qt_listbox);

    // Very small default size if specified
    shrinkable = opt.isShrinkable.value();

    connect( qt_listbox, SIGNAL( highlighted( int)), this, SLOT( slotSelected(int)));
}


void YQSelectionBox::setLabel(const YCPString &label)
{
    qt_label->setText(fromUTF8(label->value()));
    YSelectionBox::setLabel(label);
}


long YQSelectionBox::nicesize(YUIDimension dim)
{
    if (dim == YD_HORIZ)
    {
	int hintWidth = qt_label->sizeHint().width() + frameWidth();

	return max( MIN_WIDTH, hintWidth );
    }
    else
    {
	int hintHeight	 = qt_label->sizeHint().height();
	int visibleLines	 = shrinkable ? SHRINKABLE_VISIBLE_LINES : DEFAULT_VISIBLE_LINES;
	hintHeight 	+= visibleLines * qt_listbox->fontMetrics().lineSpacing();
	hintHeight	+= qt_listbox->frameWidth() * 2;

	return max( MIN_HEIGHT, hintHeight );
    }
}


void YQSelectionBox::setSize(long newwidth, long newheight)
{
    resize(newwidth, newheight);
}


void YQSelectionBox::setEnabling(bool enabled)
{
    qt_label->setEnabled(enabled);
    qt_listbox->setEnabled(enabled);
    qt_listbox->triggerUpdate(true);
}


void YQSelectionBox::itemAdded(const YCPString& string, int index, bool selected)
{
    qt_listbox->insertItem(fromUTF8(string->value()));
    if (selected) qt_listbox->setCurrentItem(index);
}


int YQSelectionBox::getCurrentItem()
{
    return qt_listbox->currentItem();
}


void YQSelectionBox::setCurrentItem(int index)
{
    qt_listbox->setCurrentItem(index);
}


bool YQSelectionBox::setKeyboardFocus()
{
    qt_listbox->setFocus();

    return true;
}


bool YQSelectionBox::eventFilter( QObject *obj, QEvent *ev )
{
    if ( ev->type() == QEvent::KeyPress )
    {
	QKeyEvent *event = (QKeyEvent *) ev;

	if ( ( event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter ) &&
	     ( event->state() == 0 || event->state() == Qt::Keypad ) )
	{
	    YQDialog *dia = (YQDialog *) yDialog();

	    if ( dia )
	    {
		(void) dia->activateDefaultButton();
		return true;
	    }
	}
    }
    
    return QWidget::eventFilter( obj, ev);   
}


// slots

void YQSelectionBox::slotSelected(int i)
{
    if (getNotify())
	yuiqt->returnNow(YUIInterpreter::ET_WIDGET, this);
}


#include "YQSelectionBox.moc.cc"
