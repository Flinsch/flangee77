#include "CoutLogger.h"

#include <CoreLabs/strings.h>

#include <iostream>
#include <iomanip>



namespace tl7::reporting {



    enum ColorCode
    {
        Black = 0,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        LightGray,
        DarkGray,
        LightRed,
        LightGreen,
        LightYellow,
        LightBlue,
        LightMagenta,
        LightCyan,
        White,

        None = Black,
        Default = Black,
        Success = Green,
        Warning = Yellow,
        Error = Red,
        Info = LightBlue,
        Code = Cyan,
    }; // enum ColorCode



    static auto& _cout()
    {
        //return std::wcout;
        return std::cout;
    }

    template <class Tcout>
    static Tcout& operator<<(Tcout& cout, ColorCode color_code)
    {
        cout << "\033";
        if (color_code == ColorCode::Black)
        {
            cout << "[0m";
        }
        else
        {
            char c = color_code > 0x7 ? '1' : '0';
            cout << "[" << c << ";" << std::to_string((color_code & 0x7) + 30).c_str() << "m";
        }
        return cout;
    }

    template <class Tcout>
    static Tcout& operator<<(Tcout& cout, cl7::u8string_view u8s)
    {
        if constexpr (std::is_same_v<typename std::remove_reference_t<decltype(cout)>::char_type, wchar_t>)
            cout << cl7::strings::to_utfx(u8s);
        else
            cout << cl7::strings::reinterpret_utf8(u8s);
        return cout;
    }



    static cl7::u8string_view _file_path(const char* file_path)
    {
        auto my_path = __FILE__;

        size_t sep = cl7::u8string::npos;

        for (size_t i = 0; file_path[i] && my_path[i] && file_path[i] == my_path[i]; ++i)
        {
            if (file_path[i] == u8'/' || file_path[i] == u8'\\')
                sep = i;
        }

        if (sep == cl7::u8string::npos)
            return cl7::strings::reinterpret_utf8(file_path);

        return cl7::strings::reinterpret_utf8({file_path + sep + 1});
    }

    static cl7::u8string_view _directory_path(cl7::u8string_view file_path)
    {
        for (size_t i = file_path.size(); i-- > 0; )
            if (file_path[i] == u8'/' || file_path[i] == u8'\\')
                return file_path.substr(0, i + 1);
        return file_path;
    }

    static cl7::u8string_view _filename(cl7::u8string_view file_path)
    {
        for (size_t i = file_path.size(); i-- > 0; )
            if (file_path[i] == u8'/' || file_path[i] == u8'\\')
                return file_path.substr(i + 1);
        return file_path;
    }



    // #############################################################################
    // IListener Implementations
    // #############################################################################

    /**
     * Handles the start of a new test run, providing the total number of test cases
     * (multiple executions/branches due to subcases etc. are not taken into account
     * here).
     */
    void CoutLogger::on_start_run(size_t count)
    {
        _total_count = count;
        _current_index = 0;

        _buffered_results.clear();

        _current_meta = _logged_meta = nullptr;
        _carriage_position = 0;
        _continue = false;

        std::cout << "Testing" << '\n';
    }

    /**
     * Handles the start of the specified test case.
     */
    void CoutLogger::on_start_case(const Meta& meta)
    {
        _current_meta = &meta;
    }

    /**
     * Handles the specified result.
     */
    void CoutLogger::on_result(const Result& result)
    {
        if (result.origin_type == Result::OriginType::TestCase)
            return;

        if (result.is_success())
            return;

        _buffer_result(result);
    }

    /**
     * Handles the specified (test case) stats at the end of a test case.
     */
    void CoutLogger::on_end_case(const Stats& stats)
    {
        constexpr size_t LINE_WIDTH = 40;

        if (_carriage_position >= LINE_WIDTH)
        {
            if (!_continue)
                std::cout << '\n';
            _carriage_position = 0;
        }
        else if (_continue)
        {
            for (size_t p = 0; p < _carriage_position; ++p)
                std::cout << ' ';
        }

        if (stats.exception_count > 0 || stats.assertions.fail_count > 0)
            std::cout << ColorCode::Error << 'E' << ColorCode::Default;
        else if (stats.checks.fail_count > 0)
            std::cout << ColorCode::Error << 'F' << ColorCode::Default;
        else if (stats.warning_count > 0)
            std::cout << ColorCode::Warning << 'W' << ColorCode::Default;
        else
            std::cout << '.';

        ++_current_index;
        ++_carriage_position;
        assert(_current_index <= _total_count);
        assert(_carriage_position <= LINE_WIDTH);
        if (_carriage_position >= LINE_WIDTH || _current_index >= _total_count)
        {
            const auto total_count = std::to_string(_total_count);
            std::cout << std::setw(static_cast<int>(total_count.size() + 1 + LINE_WIDTH - _carriage_position)) << _current_index << " / " << total_count << " (" << std::setw(3) << (_current_index * 100 / _total_count) << " %)";
        }

        std::cout << std::flush;
        _continue = false;

        if (!_defer_result_output)
        {
            assert(_buffered_results.size() <= 1);
            assert(_buffered_results.empty() || *_current_meta == _buffered_results.begin()->first);
            _log_results();
        }
    }

    /**
     * Handles the specified (final) stats at the end of a test run.
     */
    void CoutLogger::on_end_run(const Stats& stats)
    {
        if (_defer_result_output)
            _log_results();

        _log_final_stats(stats);
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Buffers the specified result for later output.
     */
    void CoutLogger::_buffer_result(const Result& result)
    {
        _buffered_results[*_current_meta].push_back(result);
    }

    /**
     * Logs (and clears) the buffered results.
     */
    void CoutLogger::_log_results()
    {
        _current_meta = _logged_meta = nullptr;
        for (const auto& [meta, results] : _buffered_results)
        {
            _current_meta = &meta;
            _log_results(results);
        }
        _buffered_results.clear();
    }

    /**
     * Logs the specified results.
     */
    void CoutLogger::_log_results(const std::vector<Result>& results)
    {
        for (const auto& result : results)
            _log_result(result);
    }

    /**
     * Logs the specified result.
     */
    void CoutLogger::_log_result(const Result& result)
    {
        auto& cout = _cout();

        assert(result.origin_type > Result::OriginType::TestCase);
        assert(result.is_failure());

        if (_logged_meta != _current_meta)
        {
            cout << "\n\n";
            _log_test_case(*_current_meta);
            _logged_meta = _current_meta;
            _continue = true;
        }

        if (result.context_meta != *_current_meta)
        {
            cout << "Subcase: " << ColorCode::Info << result.context_meta.stringification << ColorCode::Default << '\n';
        }
        if (result.context_meta.iteration_number >= 0)
        {
            cout << "Iteration: " << ColorCode::Info << (result.context_meta.iteration_number + 1) << ColorCode::Default << '\n';
        }
        if (result.data_string)
        {
            cout << "Data: " << ColorCode::Code << *result.data_string << ColorCode::Default << '\n';
        }

        if (result.result_meta.file_path)
        {
            _log_signature(result.result_meta);
            cout << '\n';
        }

        switch (result.origin_type)
        {
        case Result::OriginType::Check:
            cout << ColorCode::Error << "Error:" << ColorCode::Default << " check failed\n";
            break;
        case Result::OriginType::Presumption:
            cout << ColorCode::Warning << "Warning:" << ColorCode::Default << " presumption failed\n";
            break;
        case Result::OriginType::Assertion:
            cout << ColorCode::Error << "Error:" << ColorCode::Default << " assertion failed\n";
            break;
        case Result::OriginType::Exception:
            cout << ColorCode::Error << "Exception:" << ColorCode::Default << ' ';
            break;
        }

        switch (result.origin_type)
        {
        case Result::OriginType::Check:
        case Result::OriginType::Presumption:
        case Result::OriginType::Assertion:
            cout << "  " << ColorCode::Code << result.original_expression << ColorCode::Default << '\n';
            cout << "evaluated to:\n";
            cout << "  " << ColorCode::Code << result.evaluated_expression << ColorCode::Default << '\n';
            break;
        case Result::OriginType::Exception:
            cout << result.result_meta.stringification << '\n';
            break;
        }

        cout << '\n';
    }

    /**
     * Logs the specified (final) stats (at the end of a test run).
     */
    void CoutLogger::_log_final_stats(const Stats& stats) const
    {
        auto& cout = _cout();

        if (_defer_result_output || !_continue)
            cout << "\n\n";

        auto write_group = [&cout](cl7::u8string_view label, const Stats::Group& group) {
            cout << cl7::strings::rpadded(cl7::u8string(label), 11, u8' ') << ColorCode::Default;
            cout << cl7::strings::lpadded(cl7::to_string(group.total_count), 11, u8' ') << " total" << ColorCode::Default;
            cout << ColorCode::Success << cl7::strings::lpadded(cl7::to_string(group.pass_count), 11, u8' ') << " passed" << ColorCode::Default;
            if (group.fail_count > 0)
                cout << ColorCode::Error << cl7::strings::lpadded(cl7::to_string(group.fail_count), 11, u8' ') << " failed" << ColorCode::Default;
            cout << '\n';
        };

        auto write_single = [&cout](cl7::u8string_view label, unsigned count, ColorCode color_code, cl7::u8string_view suffix) {
            cout << cl7::strings::rpadded(cl7::u8string(label), 11, u8' ') << ColorCode::Default;
            cout << color_code << cl7::strings::lpadded(cl7::to_string(count), 11, u8' ') << suffix << ColorCode::Default;
            cout << '\n';
        };

        write_group(u8"Test cases:", stats.cases);
        if (stats.subcases.total_count != stats.cases.total_count)
            write_group(u8"Subcases:", stats.subcases);
        write_group(u8"Checks:", stats.checks);
        if (stats.assertions.total_count > 0)
            write_group(u8"Assertions:", stats.assertions);
        if (stats.exception_count > 0)
            write_single(u8"Exceptions:", stats.exception_count, ColorCode::Error, u8" exceptions");
        if (stats.warning_count > 0)
            write_single(u8"Warnings:", stats.warning_count, ColorCode::Warning, u8" warnings");

        const unsigned hours = stats.execution_time_msecs / (1000 * 60 * 60);
        const unsigned minutes = (stats.execution_time_msecs % (1000 * 60 * 60)) / (1000 * 60);
        const unsigned seconds = (stats.execution_time_msecs % (1000 * 60)) / 1000;
        const unsigned milliseconds = stats.execution_time_msecs % 1000;

        cout << "Done in ";
        if (hours > 0)
            cout << hours << " hours, " << minutes << " minutes";
        else if (minutes > 0)
            cout << minutes << " minutes, " << seconds << " seconds";
        else if (seconds > 0)
            cout << seconds << '.' << cl7::strings::lpadded(cl7::to_string(milliseconds), 3, '0') << " seconds";
        else
            cout << milliseconds << " milliseconds";
        cout << ".\n";

        cout << '\n';
    }

    /**
     * Logs the meta description of the specified test case (called when the first
     * problem occurs within said test case).
     */
    void CoutLogger::_log_test_case(const Meta& meta)
    {
        auto& cout = _cout();

        cout << "Test case: " << ColorCode::Info << meta.stringification << ColorCode::Default;
        cout << ' ' << ColorCode::Error << "FAILED" << ColorCode::Default;
        cout << '\n';

        cout << '\n';
    }

    /**
     * 
     */
    void CoutLogger::_log_signature(const Signature& signature)
    {
        auto& cout = _cout();

        const cl7::u8string_view file_path = _file_path(signature.file_path);
        const cl7::u8string_view directory_path = _directory_path(file_path);
        const cl7::u8string_view filename = _filename(file_path);

        cout << ColorCode::DarkGray << directory_path << ColorCode::White << filename << ColorCode::Default << '(' << signature.line_number << ')';
    }



} // namespace tl7::reporting
