# set up for easy 'require' from current directory
$LOAD_PATH << File.dirname(__FILE__)

# initialize things
begin
    Demo.init
    Demo.createWindow("Rubylicious Demo Engine", 1280, 720)
    v = Demo.setVSync(-1) # adaptive vsync
    puts "Running in #{Demo.width}x#{Demo.height}, vsync: " + %w(off on adaptive)[v]
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