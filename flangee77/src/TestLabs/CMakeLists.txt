
target_sources(flangee77
    PRIVATE
    Context.cpp
    Context.h
    internals.cpp
    internals.h
    macros.h
    Meta.h
    Result.cpp
    Result.h
    ResultBuilder.cpp
    ResultBuilder.h
    Signature.cpp
    Signature.h
    Stats.cpp
    Stats.h
    SubcaseBranchSwitch.cpp
    SubcaseBranchSwitch.h
    SubcaseContext.cpp
    SubcaseContext.h
    TestCase.cpp
    TestCase.h
    TestSuite.cpp
    TestSuite.h

    exceptions/assertion_exception.h

    reporting/CoutLogger.cpp
    reporting/CoutLogger.h
    reporting/IListener.h
    reporting/Reporter.cpp
    reporting/Reporter.h
)
