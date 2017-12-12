#include "colorbutton.h"
#include "colorgrid.h"
#include "widgetpopup.h"

#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QPushButton>
#include <QLayout>


namespace QSint
{


ColorButton::ColorButton(QWidget *parent)
	: QToolButton(parent),
	m_modeLeft(PM_COLORGRID_DIALOG),
	m_modeRight(PM_COLORDIALOG),
	m_tooltipMode(TM_NAMED_HEX_COLOR),
    m_labelMode(TM_NAMED_COLOR),
	m_cellSize(12)
{
	m_colorScheme = &namedColorsBase();

    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    setPopupMode(QToolButton::MenuButtonPopup);

	setColor(Qt::white);
}

ColorButton::~ColorButton()
{
}

void ColorButton::setColor(const QColor& color)
{
	m_color = color;

	QPixmap pm(iconSize());
	drawColorItem(pm, color);
	setIcon(QIcon(pm));

    setText(getColorName(m_labelMode, m_color));

    if (m_tooltipMode != TM_NONE)
        setToolTip(getColorName(m_tooltipMode, m_color));
}

void ColorButton::drawColorItem(QPixmap &pm, const QColor& color)
{
	QPainter p(&pm);

	if (!color.isValid())
	{
		p.setBrush(Qt::white);
		p.setPen(QPen(Qt::black, 1));
		p.drawRect(pm.rect().adjusted(0, 0, -1, -1));
		p.setPen(QPen(Qt::red, 1));
		p.drawLine(pm.rect().topLeft(), pm.rect().bottomRight());
	}
	else
	{
		p.setBrush(color);
		p.setPen(palette().color(QPalette::Shadow));
		p.drawRect(pm.rect().adjusted(0, 0, -1, -1));
    }
}

QString ColorButton::getColorName(ColorButton::TextMode tm, const QColor &color) const
{
    if (!color.isValid())
    {
        return tr("None");
    }

    QString namedColor = m_colorScheme->colorName(color);

    switch (tm)
    {
    case TM_NAMED_COLOR:
        return namedColor;

    case TM_HEX_COLOR:
        return color.name();

    case TM_NAMED_HEX_COLOR:
        if (namedColor == color.name())
            return namedColor;
        else
            return namedColor + " (" + color.name() + ")";

    default:;
    }

    return "";
}

void ColorButton::setPickModeLeft(PickMode mode)
{
	m_modeLeft = mode;
}

void ColorButton::setPickModeRight(PickMode mode)
{
	m_modeRight = mode;
}

void ColorButton::resizeEvent(QResizeEvent * /*event*/)
{
	setColor(m_color);
}

void ColorButton::mousePressEvent(QMouseEvent * event)
{
	QToolButton::mousePressEvent(event);

	event->accept();
	setDown(false);

	int mod;

	switch (event->button())
	{
	case Qt::LeftButton:
		mod = m_modeLeft;
		break;
	case Qt::RightButton:
		mod = m_modeRight;
		break;
	default:
		return;
	}

	switch (mod)
	{
	case PM_COLORDIALOG:
	{
		QColor c = QColorDialog::getColor(m_color, this);
		if (c.isValid())
		{
			setColor(c);
			emit colorChanged(c);
		}
	}
	break;

	case PM_COLORGRID:
	case PM_COLORGRID_DIALOG:
	{
		ColorGrid *grid = new ColorGrid();
		grid->setPickByDrag(false);
		grid->setClickMode(ColorGrid::CM_RELEASE);

		if (m_colorScheme->gridWidth > 0)
		{
			grid->setAutoSize(false);
			grid->setWidthInCells(m_colorScheme->gridWidth);
		}
		else
			grid->setAutoSize(true);

		//            grid->setColorScheme(*m_colorScheme);
		grid->setScheme(&m_colorScheme->colors);

		grid->setCellSize(m_cellSize);
		connect(grid, SIGNAL(picked(const QColor &)), this, SLOT(setColor(const QColor&)));
		connect(grid, SIGNAL(picked(const QColor &)), this, SIGNAL(colorChanged(const QColor&)));

		WidgetPopup *popup = new WidgetPopup();
		popup->setWidget(grid);

		if (mod == PM_COLORGRID_DIALOG)
		{
			QPushButton* dialogButton = new QPushButton(tr("Custom..."));
			popup->addWidget(dialogButton);

			connect(dialogButton, SIGNAL(clicked()), this, SLOT(onDialogButton()));
			connect(dialogButton, SIGNAL(clicked()), popup, SLOT(close()));
		}

		connect(grid, SIGNAL(accepted()), popup, SLOT(close()));
		popup->show(mapToGlobal(rect().bottomLeft()));
	}
	break;

	default:;
	}
}

void ColorButton::onDialogButton()
{
	QColor c = QColorDialog::getColor(m_color, this);
	if (c.isValid())
	{
		setColor(c);
		emit colorChanged(c);
	}
}

void ColorButton::setColorScheme(const NamedColorsScheme &scheme)
{
	m_colorScheme = &scheme;
}

void ColorButton::setCellSize(int size)
{
	m_cellSize = size;
}

void ColorButton::setTooltipMode(TextMode tm)
{
	m_tooltipMode = tm;
}

void ColorButton::setLabelMode(TextMode tm)
{
    m_labelMode = tm;
}


} // namespace
