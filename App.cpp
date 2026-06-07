#include "App.h"
#include "Scoring.h"

#include <fstream>
#include <iostream>

#define WIN_W 640
#define WIN_H 560
#define TOP_RESULTS 10
#define PADDING 32.f
#define INPUT_H 44.f
#define ROW_H 38.f
#define LABEL_FONT_SZ 14.f
#define RESULT_FONT_SZ 18.f
#define TITLE_FONT_SZ 22.f

nameSearch::nameSearch(const std::string& namesFile)
    : ml::Application(sf::VideoMode({WIN_W, WIN_H}), "Name Search")
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
		{
			m_names.push_back(line);
		}
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

    m_title.setFont(m_font);
    m_title.setCharacterSize(static_cast<unsigned>(TITLE_FONT_SZ));
    m_title.setFillColor(sf::Color(240, 240, 255));
    m_title.setString("Name Search");
    m_title.setPosition({PADDING, PADDING - 4.f});
    addComponent(m_title);

    m_label.setFont(m_font);
    m_label.setCharacterSize(static_cast<unsigned>(LABEL_FONT_SZ));
    m_label.setFillColor(sf::Color(160, 200, 255));
    m_label.setString("Start typing a name...");
    m_label.setPosition({PADDING, PADDING + TITLE_FONT_SZ + 12.f});
    addComponent(m_label);

    float inputY = m_label.getPosition().y + LABEL_FONT_SZ + 6.f;
    m_input.setPosition({PADDING, inputY});
    m_input.setSize({static_cast<float>(WIN_W) - PADDING * 2.f, INPUT_H});
    addComponent(m_input);

    float rowsStartY = inputY + INPUT_H + 16.f;
    m_rowBg.resize(TOP_RESULTS);
    m_rowText.resize(TOP_RESULTS);

    for (int i = 0; i < TOP_RESULTS; ++i) 
	{
        float y = rowsStartY + i * (ROW_H + 4.f);

        ml::Rectangle& bg = m_rowBg[static_cast<std::size_t>(i)];

        bg.setSize({static_cast<float>(WIN_W) - PADDING * 2.f, ROW_H});
        bg.setPosition({PADDING, y});
        sf::Color bgCol = (i % 2 == 0) ? sf::Color(40, 44, 68) : sf::Color(30, 34, 56);
        bg.setFillColor(bgCol);
        bg.disableFlag(ml::Flag::HIDDEN);
        addComponent(bg);

        ml::Text& txt = m_rowText[static_cast<std::size_t>(i)];
        txt.setFont(m_font);
        txt.setCharacterSize(static_cast<unsigned>(RESULT_FONT_SZ));
        txt.setFillColor(sf::Color(220, 230, 255));
        txt.setString("");
        txt.setPosition({PADDING + 12.f, y + (ROW_H - RESULT_FONT_SZ) / 2.f});
        addComponent(txt);

    }

    m_hint.setFont(m_font);
    m_hint.setCharacterSize(static_cast<unsigned>(RESULT_FONT_SZ));
    m_hint.setFillColor(sf::Color(120, 120, 160));
    m_hint.setString("No matches found.");
    m_hint.setPosition({PADDING + 12.f, rowsStartY + 8.f});
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
    m_results = topK(m_names, m_query, TOP_RESULTS);
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

    for (int i = 0; i < TOP_RESULTS; ++i) 
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
    for (int i = 0; i < TOP_RESULTS; ++i) 
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
