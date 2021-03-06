/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 */
#ifndef DUNEDAQ__APPFWK__CMD_CMD_STRUCTS_HPP
#define DUNEDAQ__APPFWK__CMD_CMD_STRUCTS_HPP

#include <nlohmann/json.hpp>
#include <map>
#include <string>
#include <vector>

namespace dunedaq::appfwk::cmd {

    /// @brief The set of expected command identifiers
    enum class Id: unsigned {
        exec,
        init,
        conf,
        start,
        stop,
        scrap,
        fina,
        term,
        undef,
    };
    struct IdNames {
        static constexpr char const* exec = "exec";
        static constexpr char const* init = "init";
        static constexpr char const* conf = "conf";
        static constexpr char const* start = "start";
        static constexpr char const* stop = "stop";
        static constexpr char const* scrap = "scrap";
        static constexpr char const* fina = "fina";
        static constexpr char const* term = "term";
        static constexpr char const* undef = "undef";
    };
    /// Convert a Id to a string.
    inline
    std::string str(const Id& id) {
        const std::map<Id, std::string> id2s = {
            { Id::exec, "exec" },
            { Id::init, "init" },
            { Id::conf, "conf" },
            { Id::start, "start" },
            { Id::stop, "stop" },
            { Id::scrap, "scrap" },
            { Id::fina, "fina" },
            { Id::term, "term" },
            { Id::undef, "undef" },
        };
        auto it = id2s.find(id);
        if (it == id2s.end()) {
            // fixme: replace with ERS
            throw std::runtime_error("unknown Id enum");
        }
        return it->second;
    }
    /// Convert a string to a Id
    inline
    Id toId(const std::string& id) {
        const std::map<std::string, Id> s2id = {
            { "exec", Id::exec },
            { "init", Id::init },
            { "conf", Id::conf },
            { "start", Id::start },
            { "stop", Id::stop },
            { "scrap", Id::scrap },
            { "fina", Id::fina },
            { "term", Id::term },
            { "undef", Id::undef },
        };
        auto it = s2id.find(id);
        if (it == s2id.end()) {
            throw std::runtime_error("no Id enum for " + id);
        }
        return it->second;
    }

    // @brief 
    using Data = nlohmann::json;

    /// @brief Associate data to a recipient
    struct Payload {
        std::string recipient{  };
        Data data{  };
    };


    /// @brief A command to a process
    struct Object {
        Id id{ Id::undef };
        std::vector<Payload> payloads{  };
    };


    /// @brief FSM event type for command Exec
    struct Exec {
        Id id{ Id::exec };
        Payload payload{  };
    };


    /// @brief FSM event type for command Init
    struct Init {
        Id id{ Id::init };
        Payload payload{  };
    };


    /// @brief FSM event type for command Conf
    struct Conf {
        Id id{ Id::conf };
        Payload payload{  };
    };


    /// @brief FSM event type for command Start
    struct Start {
        Id id{ Id::start };
        Payload payload{  };
    };


    /// @brief FSM event type for command Stop
    struct Stop {
        Id id{ Id::stop };
        Payload payload{  };
    };


    /// @brief FSM event type for command Scrap
    struct Scrap {
        Id id{ Id::scrap };
        Payload payload{  };
    };


    /// @brief FSM event type for command Fina
    struct Fina {
        Id id{ Id::fina };
        Payload payload{  };
    };


    /// @brief FSM event type for command Term
    struct Term {
        Id id{ Id::term };
        Payload payload{  };
    };


    /// @brief FSM event type for command Undef
    struct Undef {
        Id id{ Id::undef };
        Payload payload{  };
    };


} // namespace dunedaq::appfwk::cmd

#endif // DUNEDAQ__APPFWK__CMD_CMD_STRUCTS_HPP