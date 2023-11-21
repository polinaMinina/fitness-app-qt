#include "lightpalette.h"

QPalette lightPalette::setLightPalette()
{
    QPalette Palette;

    Palette.setColor(QPalette::Window, QColor(245, 245, 245));
    Palette.setColor(QPalette::WindowText, Qt::black);
    Palette.setColor(QPalette::Base, Qt::white);
    Palette.setColor(QPalette::AlternateBase, QColor(220, 220, 220));
    Palette.setColor(QPalette::ToolTipBase, Qt::white);
    Palette.setColor(QPalette::ToolTipText, Qt::black);
    Palette.setColor(QPalette::Text, Qt::black);
    Palette.setColor(QPalette::Button, QColor(240, 240, 240));
    Palette.setColor(QPalette::ButtonText, Qt::black);
    Palette.setColor(QPalette::BrightText, Qt::red);
    Palette.setColor(QPalette::Link, QColor(0, 122, 204));
    Palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
    Palette.setColor(QPalette::HighlightedText, Qt::black);

    return Palette;
}
