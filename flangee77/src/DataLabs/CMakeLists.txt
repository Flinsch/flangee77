
target_sources(flangee77
    PRIVATE
    Base64.cpp

    compression/Deflate.cpp

    ini/Ini.cpp
    ini/IniReader.cpp
    ini/IniWriter.cpp
    ini/Section.cpp
    ini/Value.cpp

    ini/detail/AnyOtherMatcher.cpp
    ini/detail/BooleanMatcher.cpp
    ini/detail/Builder.cpp
    ini/detail/Generator.cpp
    ini/detail/Lexer.cpp

    json/Json.cpp
    json/JsonReader.cpp
    json/JsonWriter.cpp

    json/detail/Builder.cpp
    json/detail/Generator.cpp
    json/detail/IdentifierMatcher.cpp
    json/detail/Lexer.cpp
    json/detail/UnescaperErrorHandler.cpp

    json/util/Escaper.cpp
    json/util/Unescaper.cpp
    json/util/Validator.cpp

    syntax/CstNode.cpp
    syntax/Diagnostic.cpp
    syntax/Diagnostics.cpp
    syntax/GenericLexer.cpp
    syntax/GrammarAnalyzer.cpp
    syntax/Lexer.cpp
    syntax/matchers.cpp
    syntax/ParseStage.cpp
    syntax/ProductionRule.cpp
    syntax/ProductionRuleCollection.cpp
    syntax/SourceAwareDiagnostics.cpp
    syntax/SourceContext.cpp
    syntax/SourceLocation.cpp
    syntax/TerminalSymbol.cpp
    syntax/TerminalSymbolCollection.cpp
    syntax/TokenReader.cpp
    syntax/Vocabulary.cpp

    xml/Attribute.cpp
    xml/Document.cpp
    xml/Element.cpp
    xml/Node.cpp
    xml/Text.cpp
    xml/XmlReader.cpp
    xml/XmlWriter.cpp

    xml/detail/Builder.cpp
    xml/detail/Generator.cpp
    xml/detail/Lexer.cpp
    xml/detail/UnescaperErrorHandler.cpp

    xml/util/Escaper.cpp
    xml/util/Unescaper.cpp
)
