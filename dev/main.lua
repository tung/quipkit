require "ltk"

hello = ltk.button{
    text = "Hello, world!",
    command = function ()
        print("Hello, world!")
        ltk.exit()
    end
}
ltk.pack{ hello }

ltk.mainloop()
