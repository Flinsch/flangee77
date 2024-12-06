#include "Registry.h"

#include "../logging/StandardLogger.h"

#include <CoreLabs/strings.h>
#include <CoreLabs/sstream.h>



namespace cl7 {
namespace profiling {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Begins and returns the sample with the specified name.
     */
    Sample* Registry::_begin_sample(const cl7::astring& sample_name)
    {
        auto it = _sample_lookup.find( { sample_name, _last_opened_sample } );
        if ( it == _sample_lookup.end() )
        {
            auto sample_ptr = std::make_unique<Sample>( sample_name, _last_opened_sample );
            _samples.emplace_back( std::move(sample_ptr) );
            auto p = _sample_lookup.insert( std::pair<SampleKey, SampleHandler>( { sample_name, _last_opened_sample }, { _samples.back().get() } ) );
            assert( p.second );
            it = p.first;
        }

        SampleHandler& sample_handler = it->second;

        sample_handler.open_sample();
        ++_open_sample_count;
        _last_opened_sample = sample_handler.sample;

        return _last_opened_sample;
    }

    /**
     * Ends the specified sample.
     */
    void Registry::_end_sample(Sample* sample)
    {
        assert( sample );
        assert( sample == _last_opened_sample );
        assert( _open_sample_count > 0 );
        assert( _open_sample_count == sample->parent_count + 1 );

        auto it = _sample_lookup.find( { sample->name, sample->parent_sample } );
        if ( it == _sample_lookup.end() )
        {
            assert( false );
            return;
        }

        SampleHandler& sample_handler = it->second;

        sample_handler.close_sample();
        --_open_sample_count;
        _last_opened_sample = sample->parent_sample;
    }

    /**
     * "Closes" the current frame by updating the samples accordingly, implicitly
     * starting a new frame.
     */
    void Registry::end_frame_and_update_stats(unsigned frame_usecs)
    {
        for ( auto& p : _sample_lookup )
            p.second.update_sample( frame_usecs );
    }

    /**
     * "Dumps" the profiling/sample stats to the specified logger.
     */
    void Registry::dump(cl7::logging::Logger* logger) const
    {
        if ( _samples.empty() )
            return;

        if ( !logger )
            logger = &logging::StandardLogger::instance();

        cl7::sstream ss;

        ( ss << TEXT("  Min  ") ).put( TEXT(' ') );
        while ( ss.str().length() % 14 )
            ss.put( TEXT(' ') );
        ( ss << TEXT("  Avg  ") ).put( TEXT(' ') );
        while ( ss.str().length() % 14 )
            ss.put( TEXT(' ') );
        ( ss << TEXT("  Max  ") ).put( TEXT(' ') );
        while ( ss.str().length() % 14 )
            ss.put( TEXT(' ') );
        ( ss << TEXT("  \u2300 time  ") ).put( TEXT(' ') );
        while ( ss.str().length() % 14 )
            ss.put( TEXT(' ') );
        ( ss << TEXT("  #  ") ).put( TEXT(' ') );
        while ( ss.str().length() % 10 )
            ss.put( TEXT(' ') );

        ss << TEXT("  Sample  ");

        cl7::string text = ss.str();
        for ( auto& c : text )
        {
            if ( c == TEXT(' ') )
                c = cl7::char_type(0xa0); // no-break space
        }

        logger->log( cl7::logging::LogEntry{ text, cl7::logging::LogType::Code } );
        logger->log( cl7::logging::LogEntry{ TEXT("--------------------------------------------------------------------------------"), cl7::logging::LogType::Code } );

        for ( const auto& p : _sample_lookup )
        {
            const Sample* root = p.second.sample;
            assert( root );
            if ( root->parent_sample != nullptr )
                continue;
            _log( logger, root, 0 );
        }
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * 
     */
    void Registry::_log(cl7::logging::Logger* logger, const Sample* sample, unsigned indent) const
    {
        cl7::sstream ss;

        ( ss << std::fixed << std::setprecision(2) << std::setw(6) << (sample->stats.min_pct*100.0f) << TEXT(" %") ).put( TEXT(' ') );
        while ( ss.str().length() % 14 )
            ss.put( TEXT(' ') );
        ( ss << std::fixed << std::setprecision(2) << std::setw(6) << (sample->stats.avg_pct*100.0f) << TEXT(" %") ).put( TEXT(' ') );
        while ( ss.str().length() % 14 )
            ss.put( TEXT(' ') );
        ( ss << std::fixed << std::setprecision(2) << std::setw(6) << (sample->stats.max_pct*100.0f) << TEXT(" %") ).put( TEXT(' ') );
        while ( ss.str().length() % 14 )
            ss.put( TEXT(' ') );
        if ( sample->stats.avg_usecs > 1'000'000 )
            ( ss << std::fixed << std::setprecision(1) << std::setw(6) << (static_cast<float>(sample->stats.avg_usecs/1000)*0.001f) << TEXT(" s") ).put( TEXT(' ') );
        else if ( sample->stats.avg_usecs > 1'000 )
            ( ss << std::fixed << std::setprecision(1) << std::setw(6) << (static_cast<float>(sample->stats.avg_usecs)*0.001f) << TEXT(" ms") ).put( TEXT(' ') );
        else
            ( ss << std::fixed << std::setprecision(1) << std::setw(6) << (static_cast<float>(sample->stats.avg_usecs)) << TEXT(" \u00b5s") ).put( TEXT(' ') );
        while ( ss.str().length() % 14 )
            ss.put( TEXT(' ') );
        ( ss << std::fixed << std::setprecision(0) << std::setw(5) << sample->stats.call_count << TEXT(" ") ).put( TEXT(' ') );
        while ( ss.str().length() % 10 )
            ss.put( TEXT(' ') );

        unsigned c = indent;
        while ( c-- )
            ss.put( TEXT(' ') );
        ss << cl7::strings::from_ascii( sample->name );

        cl7::string text = ss.str();
        for ( auto& c : text )
        {
            if ( c == TEXT(' ') )
                c = cl7::char_type(0xa0); // no-break space
        }

        logger->log( cl7::logging::LogEntry{ text, cl7::logging::LogType::Code } );

        for ( const auto& p : _sample_lookup )
        {
            const Sample* child = p.second.sample;
            assert( child );
            if ( child->parent_sample != sample )
                continue;
            _log( logger, child, indent + 1 );
        }
    }



} // namespace profiling
} // namespace cl7
