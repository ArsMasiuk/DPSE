#include "colorbutton.h"
#include "colorgrid.h"
#include "widgetpopup.h"

#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QPushButton>
#include <QLayout>
#include <QMenu>


namespace QSint
{


ColorButton::ColorButton(QWidget *parent)
	: QToolButton(parent),
	m_modeLeft(PM_COLORGRID_DIALOG),
	m_tooltipMode(TM_NAMED_HEX_COLOR),
    m_labelMode(TM_NAMED_COLOR)
{
	m_colorScheme = &namedColorsBase();

    //setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    setPopupMode(QToolButton::MenuButtonPopup);

	setColor(Qt::white);

    // build up menu
    QMenu *menu = new QMenu(this);
    setMenu(menu);

    m_grid = new ColorGrid(this);
    m_grid->setPickByDrag(false);
    m_grid->setClickMode(ColorGrid::CM_RELEASE);
    connect(m_grid, SIGNAL(picked(const QColor&)), this, SLOT(setColor(const QColor&)));
    connect(m_grid, SIGNAL(accepted()), menu, SLOT(hide()));

    auto gridAction = new QWidgetAction(this);
    gridAction->setDefaultWidget(m_grid);
    menu->addAction(gridAction);


    QPushButton* dialogButton = new QPushButton(tr("Choose Color..."));
    connect(dialogButton, SIGNAL(clicked()), this, SLOT(onDialogButton()));

    m_dialogButtonAction = new QWidgetAction(this);
    m_dialogButtonAction->setDefaultWidget(dialogButton);
    menu->addAction(m_dialogButtonAction);

    //
    connect(this, SIGNAL(colorChanged(QColor)), this, SIGNAL(activated(QColor)));
    connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

ColorButton::~ColorButton()
{
}

void ColorButton::setColor(const QColor& color)
{
    if (m_color != color)
    {
        m_color = color;

        QPixmap pm(iconSize());
        drawColorItem(pm, color);
        setIcon(QIcon(pm));

        setText(getColorName(m_labelMode, m_color));

        if (m_tooltipMode != TM_NONE)
            setToolTip(getColorName(m_tooltipMode, m_color));

        Q_EMIT colorChanged(color);
    }
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

void ColorButton::resizeEvent(QResizeEvent * /*event*/)
{
	setColor(m_color);
}

void ColorButton::onDialogButton()
{
	QColor c = QColorDialog::getColor(m_color, this);
	if (c.isValid())
	{
		setColor(c);
        Q_EMIT colorChanged(c);
	}
}

void ColorButton::onClicked()
{
    Q_EMIT activated(m_color);
}

void ColorButton::setColorScheme(const NamedColorsScheme &scheme)
{
	m_colorScheme = &scheme;

    if (m_colorScheme->gridWidth > 0)
    {
        m_grid->setAutoSize(false);
        m_grid->setWidthInCells(m_colorScheme->gridWidth);
    }
    else
        m_grid->setAutoSize(true);

    m_grid->setScheme(&m_colorScheme->colors);

    //m_grid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void ColorButton::setCellSize(int size)
{
    m_grid->setCellSize(size);
}

int ColorButton::cellSize() const
{
    return m_grid->cellSize();
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
