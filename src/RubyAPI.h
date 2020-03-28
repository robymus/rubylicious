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
     * Returns exception as string, if terminated with Exception, nullptr otherwise
     */
    const char *runScript(const char *mainScriptName);

    /**
     * Shuts down the Ruby VM
     */
    void shutdown();

}

#endif //RUBYLICIOUS_RUBYAPI_H
