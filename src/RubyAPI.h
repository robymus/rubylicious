#ifndef RUBYLICIOUS_RUBYAPI_H
#define RUBYLICIOUS_RUBYAPI_H

namespace Ruby {

    /**
     * Initializes the Ruby VM and API
     */
    void initialize();

    /**
     * Run main script (eg. enty point)
     * Note: should start with ./
     * Returns true, if terminated successfully, false in case of Exception (logged to stderr as well)
     */
    bool runScript(const char *mainScriptName);

    /**
     * Shuts down the Ruby VM
     */
    void shutdown();

}

#endif //RUBYLICIOUS_RUBYAPI_H
