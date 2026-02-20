
#include <TestLabs/TestSuite.h>

#include <DataLabs/xml/Document.h>
#include <DataLabs/xml/XmlReader.h>
#include <DataLabs/xml/XmlWriter.h>
#include <DataLabs/xml/util/Escaper.h>
#include <DataLabs/xml/util/Unescaper.h>

#include "../shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const dl7::xml::Document& document) { return dl7::xml::XmlWriter::to_string( document ); }
}



TESTLABS_CASE( u8"DataLabs:  xml:  util::Escaper::escape_string" )
{
    struct Entry
    {
        cl7::u8string raw_string;
        dl7::xml::util::Escaper::EscapeType escape_type;
        dl7::xml::util::Escaper::Options options;
        cl7::u8string expected_string;
    } entry;

    const std::vector<Entry> container {
        { u8"", dl7::xml::util::Escaper::EscapeType::ElementContent, {}, u8"" },
        { u8"Hello World", dl7::xml::util::Escaper::EscapeType::ElementContent, {}, u8"Hello World" },
        { u8"Hello\nWorld", dl7::xml::util::Escaper::EscapeType::ElementContent, {}, u8"Hello\nWorld" },
        { u8"<Hello World>", dl7::xml::util::Escaper::EscapeType::ElementContent, {}, u8"&lt;Hello World>" },
        { u8"<Hello World>", dl7::xml::util::Escaper::EscapeType::ElementContent, {.force_escape = true}, u8"&lt;Hello World&gt;" },
        { u8"&<>\"'", dl7::xml::util::Escaper::EscapeType::ElementContent, {}, u8"&amp;&lt;>\"'" },
        { u8"&<>\"'", dl7::xml::util::Escaper::EscapeType::ElementContent, {.force_escape = true}, u8"&amp;&lt;&gt;&quot;&apos;" },
        { u8"&<>\"'", dl7::xml::util::Escaper::EscapeType::DoubleQuotedAttributeValue, {}, u8"&amp;&lt;>&quot;'" },
        { u8"&<>\"'", dl7::xml::util::Escaper::EscapeType::SingleQuotedAttributeValue, {}, u8"&amp;&lt;>\"&apos;" },
        { u8" \t\n\r", dl7::xml::util::Escaper::EscapeType::ElementContent, {}, u8" \t\n\r" },
        { u8" \t\n\r", dl7::xml::util::Escaper::EscapeType::DoubleQuotedAttributeValue, {}, u8"    " },
        { u8" \t\n\r", dl7::xml::util::Escaper::EscapeType::SingleQuotedAttributeValue, {}, u8"    " },
        { u8"\x07", dl7::xml::util::Escaper::EscapeType::ElementContent, {}, u8"&#x07;" },
        { u8"ß", dl7::xml::util::Escaper::EscapeType::ElementContent, {}, u8"ß" },
        { u8"ß", dl7::xml::util::Escaper::EscapeType::ElementContent, {.escape_unicode = true}, u8"&#x00df;" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.raw_string )
    {
        TESTLABS_CHECK_EQ( dl7::xml::util::Escaper{}.escape_string( entry.raw_string, entry.escape_type, entry.options ), entry.expected_string );
    }
}



TESTLABS_CASE( u8"DataLabs:  xml:  util::Unescaper::unescape_string" )
{
    struct Entry
    {
        cl7::u8string xml_string;
        dl7::xml::util::Unescaper::UnescapeType unescape_type;
        cl7::u8string expected_string;
        bool expected_result = true;
    } entry;

    const std::vector<Entry> container {
        { u8"", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8"" },
        { u8"Hello World", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8"Hello World" },
        { u8"Hello\nWorld", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8"Hello\nWorld" },
        { u8"&lt;Hello World>", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8"<Hello World>" },
        { u8"&lt;Hello World&gt;", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8"<Hello World>" },
        { u8"&amp;&lt;>\"'", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8"&<>\"'" },
        { u8"&amp;&lt;&gt;&quot;&apos;", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8"&<>\"'" },
        { u8"&amp;&lt;>&quot;'", dl7::xml::util::Unescaper::UnescapeType::AttributeValue, u8"&<>\"'" },
        { u8" \t\n\r", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8" \t\n\r" },
        { u8"&#x07;", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8"\x07" },
        { u8"ß", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8"ß" },
        { u8"&#x00df;", dl7::xml::util::Unescaper::UnescapeType::ElementContent, u8"ß" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.xml_string )
    {
        TESTLABS_CHECK_EQ( dl7::xml::util::Unescaper{}.unescape_string( entry.xml_string, entry.unescape_type ), entry.expected_string );
        cl7::u8osstream oss;
        TESTLABS_CHECK_EQ( dl7::xml::util::Unescaper{}.unescape_string( oss, entry.xml_string, entry.unescape_type ), entry.expected_result );
    }
}



TESTLABS_CASE( u8"DataLabs:  xml:  XmlReader:  parse" )
{
    cl7::u8string string;
    dl7::xml::Document document;
    dl7::xml::Element* element;


    string = u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<root>\n"
"  <empty-element/>\n"
"</root>\n"
"";

    document = {};
    document.root_element().set_name(u8"root");
    document.root_element().append_element(u8"empty-element");

    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string ), document );


    string = u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<root>\n"
"  <empty-element/>\n"
"  <element-with-attributes x=\"1\" y=\"2\" />\n"
"</root>\n"
"";

    document = {};
    document.root_element().set_name(u8"root");
    document.root_element().append_element(u8"empty-element");
    element = document.root_element().append_element(u8"element-with-attributes");
    element->attributes().emplace_back(u8"x", u8"1");
    element->attributes().emplace_back(u8"y", u8"2");

    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string ), document );


    string = u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<root>\n"
"  <empty-element/>\n"
"  <element-with-attributes x=\"1\" y=\"2\" />\n"
"  <element-with-content>\n"
"    Hello \t World\n"
"  </element-with-content>\n"
"</root>\n"
"";

    document = {};
    document.root_element().set_name(u8"root");
    document.root_element().append_element(u8"empty-element");
    element = document.root_element().append_element(u8"element-with-attributes");
    element->attributes().emplace_back(u8"x", u8"1");
    element->attributes().emplace_back(u8"y", u8"2");
    element = document.root_element().append_element(u8"element-with-content");
    element->append_text(u8"\n    Hello \t World\n  ");

    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string ), document );

    element->set_text_content(u8"\n    Hello \t World\n  ");
    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string, dl7::xml::WhitespaceHandling::Preserve ), document );

    element->set_text_content(u8"     Hello   World   ");
    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string, dl7::xml::WhitespaceHandling::Replace ), document );

    element->set_text_content(u8"Hello World");
    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string, dl7::xml::WhitespaceHandling::Collapse ), document );


    string = u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<root>\n"
"  <empty-element/>\n"
"  <element-with-attributes x=\"1\" y=\"2\" />\n"
"  <element-with-content xml:space=\"preserve\">\n"
"    Hello \t World\n"
"  </element-with-content>\n"
"  <element-with-content xml:space=\"replace\">\n"
"    Hello \t World\n"
"  </element-with-content>\n"
"  <element-with-content xml:space=\"collapse\">\n"
"    Hello \t World\n"
"  </element-with-content>\n"
"  <element-with-content xml:space=\"default\">\n"
"    Hello \t World\n"
"  </element-with-content>\n"
"</root>\n"
"";

    document = {};
    document.root_element().set_name(u8"root");
    document.root_element().append_element(u8"empty-element");
    element = document.root_element().append_element(u8"element-with-attributes");
    element->attributes().emplace_back(u8"x", u8"1");
    element->attributes().emplace_back(u8"y", u8"2");
    element = document.root_element().append_element(u8"element-with-content");
    element->attributes().emplace_back(u8"xml:space", u8"preserve");
    element->append_text(u8"\n    Hello \t World\n  ");
    element = document.root_element().append_element(u8"element-with-content");
    element->attributes().emplace_back(u8"xml:space", u8"replace");
    element->append_text(u8"     Hello   World   ");
    element = document.root_element().append_element(u8"element-with-content");
    element->attributes().emplace_back(u8"xml:space", u8"collapse");
    element->append_text(u8"Hello World");
    element = document.root_element().append_element(u8"element-with-content");
    element->attributes().emplace_back(u8"xml:space", u8"default");
    element->append_text(u8"\n    Hello \t World\n  ");

    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string ), document );

    element->set_text_content(u8"\n    Hello \t World\n  ");
    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string, dl7::xml::WhitespaceHandling::Preserve ), document );

    element->set_text_content(u8"     Hello   World   ");
    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string, dl7::xml::WhitespaceHandling::Replace ), document );

    element->set_text_content(u8"Hello World");
    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string, dl7::xml::WhitespaceHandling::Collapse ), document );


    string = u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<root>\n"
"  <empty-element/>\n"
"  <element-with-attributes x=\"1\" y=\"2\" />\n"
"  <element-with-content>\n"
"    <![CDATA[ <&lt; Hello World &gt;> ]]>\n"
"  </element-with-content>\n"
"</root>\n"
"";

    document = {};
    document.root_element().set_name(u8"root");
    document.root_element().append_element(u8"empty-element");
    element = document.root_element().append_element(u8"element-with-attributes");
    element->attributes().emplace_back(u8"x", u8"1");
    element->attributes().emplace_back(u8"y", u8"2");
    element = document.root_element().append_element(u8"element-with-content");
    element->append_text(u8"\n     <&lt; Hello World &gt;> \n  ");

    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string ), document );

    element->set_text_content(u8"\n     <&lt; Hello World &gt;> \n  ");
    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string, dl7::xml::WhitespaceHandling::Preserve ), document );

    element->set_text_content(u8"      <&lt; Hello World &gt;>    ");
    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string, dl7::xml::WhitespaceHandling::Replace ), document );

    element->set_text_content(u8" <&lt; Hello World &gt;> ");
    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string, dl7::xml::WhitespaceHandling::Collapse ), document );


    string = u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<root>\n"
"  <empty-element/>\n"
"  <element-with-attributes x=\"1\" y=\"2\" />\n"
"  <parent-element foo=\"bar\">\n"
"    Hart &amp; Glücklich    \n"
"    <child-element name=\"ABC\">ABC</child-element>\n"
"    <child-element name=\"XYZ\">\n"
"      XYZ\n"
"    </child-element>\n"
"    Dies ist eine Zeile,\n"
"    \n"
"    und dies ist noch eine Zeile,\n"
"    <!-- This is a fucking comment. -->\n"
"    nach einer leeren Zeile.\n"
"    <![CDATA[ <&lt; Hello World &gt;> ]]>\n"
"  </parent-element>\n"
"</root>\n"
"";

    document = {};
    document.root_element().set_name(u8"root");
    document.root_element().append_element(u8"empty-element");
    element = document.root_element().append_element(u8"element-with-attributes");
    element->attributes().emplace_back(u8"x", u8"1");
    element->attributes().emplace_back(u8"y", u8"2");
    element = document.root_element().append_element(u8"parent-element");
    element->attributes().emplace_back(u8"foo", u8"bar");
    element->append_text(u8"\n    Hart & Glücklich    \n    ");
    element = element->append_element(u8"child-element");
    element->attributes().emplace_back(u8"name", u8"ABC");
    element->append_text(u8"ABC");
    element = element->get_parent_element()->append_element(u8"child-element");
    element->attributes().emplace_back(u8"name", u8"XYZ");
    element->append_text(u8"\n      XYZ\n    ");
    element = element->get_parent_element();
    element->append_text(u8"\n"
"    Dies ist eine Zeile,\n"
"    \n"
"    und dies ist noch eine Zeile,\n"
"    \n"
"    nach einer leeren Zeile.\n"
"     <&lt; Hello World &gt;> \n"
"  ");

    TESTLABS_CHECK_EQ( dl7::xml::XmlReader::parse( string ), document );
}



TESTLABS_CASE( u8"DataLabs:  xml:  XmlWriter:  to_string" )
{
    dl7::xml::Document document;
    dl7::xml::Element* element;
    cl7::u8string string;


    document = {};
    document.root_element().set_name(u8"root");

    string = u8"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<root/>\n"
"";

    TESTLABS_CHECK_EQ( dl7::xml::XmlWriter::to_string( document ), string );

    string = u8"<?xml version=\"1.0\" encoding=\"utf-8\"?><root/>";
    TESTLABS_CHECK_EQ( dl7::xml::XmlWriter::to_string( document, dl7::xml::XmlWriter::DEFAULT_COMPACT_FORMAT ), string );


    document = {};
    document.root_element().set_name(u8"root");
    document.root_element().ensure_attribute(u8"foo", u8"foo");
    document.root_element().ensure_attribute(u8"bar", u8"\"foo's\"");
    document.root_element().ensure_attribute(u8"foobar", u8"'\"foo\"'");

    string = u8"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<root foo=\"foo\" bar='\"foo&apos;s\"' foobar=\"'&quot;foo&quot;'\"/>\n"
"";

    TESTLABS_CHECK_EQ( dl7::xml::XmlWriter::to_string( document ), string );


    document = {};
    document.root_element().set_name(u8"root");
    element = document.root_element().append_element(u8"element");
    element->append_text(u8"< Hello \n World ß >");

    string = u8"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<root>\n"
"  <element>&lt; Hello \n World ß ></element>\n"
"</root>\n"
"";

    TESTLABS_CHECK_EQ( dl7::xml::XmlWriter::to_string( document ), string );

    string = u8"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
"<!DOCTYPE root>\r\n"
"<root>\r\n"
"\t<element>&lt; Hello \n World &#x00df; &gt;</element>\r\n"
"</root>";
    TESTLABS_CHECK_EQ( dl7::xml::XmlWriter::to_string( document, {.pretty_options = {.indentation = 0, .line_ending = dl7::xml::Format::PrettyOptions::LineEnding::CRLF, .add_empty_line = false}, .force_escape = true, .escape_unicode = true, .insert_doctype = true} ), string );


    document = {};
    document.root_element().set_name(u8"root");
    document.root_element().append_text(u8"A");
    document.root_element().append_element(u8"element");
    document.root_element().append_element(u8"element");
    document.root_element().append_text(u8"B");
    document.root_element().append_text(u8"C");
    document.root_element().append_element(u8"element");
    document.root_element().append_text(u8"D");

    string = u8"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<root>A<element/>\n"
"  <element/>BC<element/>D"
"</root>\n"
"";

    TESTLABS_CHECK_EQ( dl7::xml::XmlWriter::to_string( document ), string );


    document = {};
    document.root_element().set_name(u8"root");
    element = document.root_element().append_element(u8"element");
    element->append_element(u8"child")->set_text_content(u8"A");
    element->append_element(u8"child");
    element->append_element(u8"child")->set_text_content(u8"C");

    string = u8"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<root>\n"
"  <element>\n"
"    <child>A</child>\n"
"    <child/>\n"
"    <child>C</child>\n"
"  </element>\n"
"</root>\n"
"";

    TESTLABS_CHECK_EQ( dl7::xml::XmlWriter::to_string( document ), string );
}
