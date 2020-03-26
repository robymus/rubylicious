# set up for easy 'require' from current directory
$LOAD_PATH << File.dirname(__FILE__)

Demo.renderFinal {
    puts "rendering final"

    t1 = Demo.renderTexture("T1") {
        puts "but first T1"

    }

    puts "T1 name: #{t1.name}"

    t2 = Demo.renderTexture("T2") {
        puts "end then T2"
    }

    puts "T2 name: #{t2.name}"

}



