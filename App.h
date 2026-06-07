#ifndef APP_H
#define APP_H

#include <Malena/Engine/App/Application.h>
#include <Malena/Graphics/Text/TextInput.h>
#include <Malena/Graphics/Text/Text.h>
#include <Malena/Graphics/Primitives/Rectangle.h>

#include <string>
#include <vector>

class nameSearch : public ml::Application
{
    std::vector<std::string> m_names;
    std::string m_query;
    std::vector<std::string> m_results;

    ml::Text m_title;
    ml::Text m_label;
    ml::TextInput m_input;

    std::vector<ml::Rectangle> m_rowBg;
    std::vector<ml::Text> m_rowText;

    ml::Text m_hint;

    sf::Font m_font;

public:
    explicit nameSearch(const std::string& namesFile);

private:
    void onInit() override;
    void onReady() override;

    void refreshQuery();
    void updateRows();
    void setRowsVisible(bool visible);
};

#endif
