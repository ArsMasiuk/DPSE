#include "colordefs.h"


namespace QSint
{


ColorList* defaultColors()
{
    static ColorList s_defaultColors;

    if (!s_defaultColors.isEmpty())
        return &s_defaultColors;

    const int step = 32;

    for (int b = 0; b <= 256; b += 16) {
        if (b == 256) b = 255;

        s_defaultColors.append(QColor(b,b,b));
    }

    for (int r = 0; r <= 256; r += step) {
        if (r == 256) r = 255;

        for (int g = 0; g <= 256; g += step) {
            if (g == 256) g = 255;

            for (int b = 0; b <= 256; b += step) {
                if (b == 256) b = 255;

                s_defaultColors.append(QColor(r,g,b));
            }
        }
    }

    return &s_defaultColors;
}

ColorList* defaultColors2()
{
    static ColorList s_defaultColors2;

    if (!s_defaultColors2.isEmpty())
        return &s_defaultColors2;

    const int step = 64;

    for (int b = 0; b <= 256; b += 16) {
        if (b == 256) b = 255;

        s_defaultColors2.append(QColor(b,b,b));
    }

    for (int r = 0; r <= 256; r += step) {
        if (r == 256) r = 255;

        for (int g = 0; g <= 256; g += step) {
            if (g == 256) g = 255;

            for (int b = 0; b <= 256; b += step) {
                if (b == 256) b = 255;

                s_defaultColors2.append(QColor(r,g,b));
            }
        }
    }

    return &s_defaultColors2;
}

ColorList* baseColors()
{
    static ColorList s_baseColors;

    if (!s_baseColors.isEmpty())
        return &s_baseColors;

    for (int i = Qt::black; i <= Qt::darkYellow; i++)
        s_baseColors.append((Qt::GlobalColor)i);

    return &s_baseColors;
}

ColorList* namedColors()
{
    static ColorList s_namedColors;

    if (!s_namedColors.isEmpty())
        return &s_namedColors;

    QStringList names = QColor::colorNames();
    for (int i = 0; i < names.size(); i++)
        s_namedColors.append(QColor(names.at(i)));

    return &s_namedColors;
}


ColorList* openOfficeColors()
{
	static ColorList s_namedColors;

	if (!s_namedColors.isEmpty())
		return &s_namedColors;

	s_namedColors << "#fff" << "#eee" << "#ddd" << "#ccc" << "#b2b2b2" << "#999" << "#808080" << "#666" << "#333" << "#1c1c1c" << "#111" << "#000";
	s_namedColors << "#ff9" << "#f60" << "#f33" << "#f0c" << "#f3f" << "#90f" << "#66f" << "#0cf" << "#6ff" << "#3f9" << "#9f6" << "#cf0";
	s_namedColors << "#ffff00" << "#ff9900" << "#ff6666" << "#ff3399" << "#ff66ff" << "#9933ff" << "#3333ff" << "#3399ff" << "#00ffff" << "#00ff66" << "#66ff66" << "#99ff33";
	s_namedColors << "#cc9900" << "#ff3300" << "#ff0000" << "#ff0066" << "#ff00ff" << "#6600ff" << "#0000ff" << "#0066ff" << "#00cccc" << "#00cc33" << "#00cc00" << "#66ff00";

	return &s_namedColors;
}


}

