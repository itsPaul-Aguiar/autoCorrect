#include "App.h"
#include "Scoring.h"

#include <fstream>
#include <iostream>

#define winW 640
#define winH 560
#define topResults 10
#define padding 32.f
#define inputH 44.f
#define rowH 38.f
#define labelFont 14.f
#define resultFont 18.f
#define titleFont 22.f

nameSearch::nameSearch(const std::string& namesFile)
    : ml::Application(sf::VideoMode({winW, winH}), "Name Search")
{
    std::ifstream file(namesFile);

    if (!file.is_open()) 
	{
        std::cerr << "Could not open: " << namesFile << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) 
	{
        while (!line.empty() && (line.back() == '\r' || line.back() == ' '))
		{
            line.pop_back();
		}

        if (!line.empty())
			m_names.push_back(line);
    }

    std::cout << "Loaded " << m_names.size() << " names.\n";
}

void nameSearch::onInit()
{
    std::vector<std::string> fontPaths = 
	{
        "/System/Library/Fonts/HelveticaNeue.ttc",
        "/System/Library/Fonts/Menlo.ttc",
        "assets/font.ttf"
    };

    bool fontLoaded = false;
    for (const auto& fp : fontPaths) 
	{
        if (m_font.openFromFile(fp)) 
		{
            fontLoaded = true;
            break;
        }
    }

    if (!fontLoaded)
	{
        std::cerr << "font error";
	}

    setBackgroundColor(sf::Color(40, 40, 38)); // gruvbox dark1

    m_title.setFont(m_font);
    m_title.setCharacterSize(static_cast<unsigned>(titleFont));
    m_title.setFillColor(sf::Color(215, 223, 229)); // gruvbox fg
    m_title.setString("Name Search");
    m_title.setPosition({padding, padding - 4.f});
    addComponent(m_title);

    m_label.setFont(m_font);
    m_label.setCharacterSize(static_cast<unsigned>(labelFont));
    m_label.setFillColor(sf::Color(168, 154, 136)); // gruvbox gray
    m_label.setString("Start typing a name...");
    m_label.setPosition({padding, padding + titleFont + 12.f});
    addComponent(m_label);

    float inputY = m_label.getPosition().y + labelFont + 6.f;
    m_input.setFont(m_font);
    m_input.setPosition({padding, inputY});
    m_input.setSize({static_cast<float>(winW) - padding * 2.f, inputH});
    m_input.setBorderFocused(sf::Color(250, 149, 70)); // gruvbox orange
    m_input.setBorderIdle(sf::Color(142, 149, 161)); // gruvbox gray
    m_input.setPlaceholderColor(sf::Color(142, 149, 161)); // gruvbox gray
    m_input.setBgIdle(sf::Color(60, 56, 54)); // gruvbox dark0
    m_input.setBgFocused(sf::Color(60, 56, 54)); // gruvbox dark0
    addComponent(m_input);

    float rowsStartY = inputY + inputH + 16.f;
    m_rowBg.resize(topResults);
    m_rowText.resize(topResults);

    for (int i = 0; i < topResults; ++i) 
	{
        float y = rowsStartY + i * (rowH + 4.f);

        ml::Rectangle& bg = m_rowBg[static_cast<std::size_t>(i)];

        bg.setSize({static_cast<float>(winW) - padding * 2.f, rowH});
        bg.setPosition({padding, y});
        sf::Color bgCol = (i % 2 == 0) ? sf::Color(60, 56, 54) : sf::Color(40, 40, 38); // gruvbox dark0/dark1
        bg.setFillColor(bgCol);
        bg.disableFlag(ml::Flag::HIDDEN);
        addComponent(bg);

        ml::Text& txt = m_rowText[static_cast<std::size_t>(i)];
        txt.setFont(m_font);
        txt.setCharacterSize(static_cast<unsigned>(resultFont));
        txt.setFillColor(sf::Color(235, 239, 244)); // gruvbox fg
        txt.setString("");
        txt.setPosition({padding + 12.f, y + (rowH - resultFont) / 2.f});
        addComponent(txt);

    }

    m_hint.setFont(m_font);
    m_hint.setCharacterSize(static_cast<unsigned>(resultFont));
    m_hint.setFillColor(sf::Color(142, 149, 161)); // gruvbox gray
    m_hint.setString("No matches found.");
    m_hint.setPosition({padding + 12.f, rowsStartY + 8.f});
    m_hint.enableFlag(ml::Flag::HIDDEN);
    addComponent(m_hint);

    setRowsVisible(false);
}

void nameSearch::onReady()
{
    m_input.onChange([this](const std::string&) 
	{
        refreshQuery();
    });
}

void nameSearch::refreshQuery()
{
    std::string current = m_input.getValue();
    if (current == m_query) 
	{
		return;
    }

    m_query = current;
    m_results = topK(m_names, m_query, topResults);
    updateRows();
}

void nameSearch::updateRows()
{
    if (m_query.empty()) 
	{
        setRowsVisible(false);
        m_hint.enableFlag(ml::Flag::HIDDEN);
        return;
    }

    if (m_results.empty()) 
	{
        setRowsVisible(false);
        m_hint.disableFlag(ml::Flag::HIDDEN);
        return;
    }

    m_hint.enableFlag(ml::Flag::HIDDEN);

    for (int i = 0; i < topResults; ++i) 
	{
        std::size_t sz = static_cast<std::size_t>(i);
        if (i < static_cast<int>(m_results.size())) 
		{
            m_rowBg[sz].disableFlag(ml::Flag::HIDDEN);
            m_rowText[sz].setString(m_results[sz]);
            m_rowText[sz].disableFlag(ml::Flag::HIDDEN);

        } else 
		{
            m_rowBg[sz].enableFlag(ml::Flag::HIDDEN);
            m_rowText[sz].setString("");
            m_rowText[sz].enableFlag(ml::Flag::HIDDEN);
        }
    }
}

void nameSearch::setRowsVisible(bool visible)
{
    for (int i = 0; i < topResults; ++i) 
	{
        std::size_t sz = static_cast<std::size_t>(i);

        if (visible) 
		{
            m_rowBg[sz].disableFlag(ml::Flag::HIDDEN);
            m_rowText[sz].disableFlag(ml::Flag::HIDDEN);
        } else 
		{
            m_rowBg[sz].enableFlag(ml::Flag::HIDDEN);
            m_rowText[sz].enableFlag(ml::Flag::HIDDEN);
        }
    }
}
