# set up for easy 'require' from current directory
$LOAD_PATH << File.dirname(__FILE__)

# initialize things
begin
    Demo.init
    Demo.setGLAttribute('CONTEXT_MAJOR_VERSION', 4);
    Demo.setGLAttribute('CONTEXT_MINOR_VERSION', 0);
    Demo.setGLAttribute('CONTEXT_PROFILE_MASK', 'CORE');
    Demo.createWindow("Rubylicious Demo Engine", 1920, 1080);
rescue RuntimeError => ex
    puts "Demo initialization failed: #{ex.message}"
    Demo.messageBox("Error", ex.message, "error")
    Demo.shutdown
    exit
end

# main loop
running = true
while running do
    # TODO: render something

    Demo.pollEvents do |event|
        running = false if event[:type] == 'QUIT' # window closed
        running = false if event[:type] == 'KEYDOWN' && event[:key] == 27 # ESC
    end
end

# cleanup
Demo.shutdown