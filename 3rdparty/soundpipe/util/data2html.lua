#!/usr/bin/env lua

-- Parse a lua table in modules/data and write text
-- to run: lua sp.lua "foo"

sptbl = {}
module = arg[1]
dofile(string.format("modules/data/%s.lua", module))
PG = { name = module }

function PG.printheader(self, str)
    io.write("<div class=\"row\">\n")
    io.write("<h2>" .. str .. "</h2>")
    io.write("</div>\n")
end

function PG.printoption(self, str)
    io.write("<i>" .. str .. ":</i> ")
end

function PG.title(self, sp)
    io.write("<i><h1>" .. self.name .. "</h1></i>\n")
end

function PG.desc(self, sp)
    str,_ =  string.gsub(sp[self.name].description, "\n\n","<br>")
    print(str)
    io.write("<div class=\"row\"><br></div>\n")
end

function PG.createf(self, sp)
    local tbl = sp[self.name]
    print(string.format("%s(sp_%s **%s)", 
        tbl.func.create, self.name, self.name, tbl.func.init))
end

function PG.destroyf(self, sp)
    local tbl = sp[self.name]
    print(string.format("%s(sp_%s **%s)", 
        tbl.func.destroy, self.name, self.name, tbl.func.init))
end

function PG.initf(self, sp)
    local tbl = sp[self.name]
    
    io.write(string.format("%s(sp_data *sp, sp_%s *%s", 
    tbl.func.init, self.name, self.name))
    
    if(tbl.params.mandatory ~= nil) then
    
        for _, v in pairs(tbl.params.mandatory) do
            if(string.byte(v.type, string.len(v.type)) == 42) then
    	        arg = string.format(", %s%s", v.type, v.name)
            else
    	        arg = string.format(", %s %s", v.type, v.name)
            end
            io.write(arg)
        end
    end

    io.write(")\n")
end

function PG.genf(self, sp)
    local tbl = sp[self.name]
    
    io.write(string.format("%s(sp_data *sp, sp_ftbl *ft ", tbl.func.name)) 
    
    if(tbl.params ~= nil) then
        for _, v in pairs(tbl.params) do
            if(string.byte(v.type, string.len(v.type)) == 42) then
    	        arg = string.format(", %s%s", v.type, v.name)
            else
    	        arg = string.format(", %s %s", v.type, v.name)
            end
            io.write(arg)
        end
    end

    io.write(")\n")
end

function PG.computef(self, sp)
    local tbl = sp[self.name]
    io.write(string.format("%s(sp_data *sp, sp_%s *%s", 
        tbl.func.compute, self.name, self.name, tbl.func.init))
        
    for _, input in pairs(tbl.inputs) do
        io.write(string.format(", SPFLOAT *%s", input.name))
    end
    
    for _, output in pairs(tbl.outputs) do
        io.write(string.format(", SPFLOAT *%s", output.name))
    end

    io.write(")\n")
end


function PG.funcs(self, sp)
    io.write("<div class=\"row\">\n")
    self:printheader("Functions")    
    io.write("</div>\n")
    io.write("<div class=\"row\">\n")
    self:createf(sp) 
    io.write("</div>\n")
    io.write("<div class=\"row\">\n")
    self:initf(sp)   
    io.write("</div>\n")
    io.write("<div class=\"row\">\n")
    self:computef(sp)
    io.write("</div>\n")
    io.write("<div class=\"row\">\n")
    self:destroyf(sp)   
    io.write("</div>\n")
    io.write("<div class=\"row\"><br></div>\n")
end

function PG.genfuncs(self, sp)
    io.write("<div class=\"row\">\n")
    self:printheader("Functions")    
    io.write("</div>\n")
    io.write("<div class=\"row\">\n")
    self:genf(sp) 
    io.write("</div>\n")
    io.write("<div class=\"row\"><br></div>\n")
end

function PG.man_params(self,sp)
    local tbl = sp[self.name].params.mandatory
	if (tbl == nil) then return end 
    self:printheader("Mandatory Parameters")
    for _, v in pairs(tbl) do
        io.write("<div class=\"row\">\n")
        self:printoption(v.name)
        io.write(v.description)
        io.write("</div>\n")
        io.write("<div class=\"row\">\n")
        io.write(string.format("\n(Recommended value: %s)\n", 
            v.default))
        io.write("</div>\n")
    end
    io.write("<div class=\"row\"><br></div>\n")
end

function PG.genparams(self,sp)
    local tbl = sp[self.name].params
	if (tbl == nil) then return end 
    self:printheader("Parameters")
    for _, v in pairs(tbl) do
        io.write("<div class=\"row\">\n")
        self:printoption(v.name)
        io.write(v.description)
        io.write("</div>\n")
        io.write("<div class=\"row\">\n")
        io.write(string.format("\n(Recommended value: %s)\n", 
            v.default))
        io.write("</div>\n")
    end
    io.write("<div class=\"row\"><br></div>\n")
 end
function PG.opt_params(self,sp)
    local tbl = sp[self.name].params.optional
    
    if (tbl == nil) then return end
    self:printheader("Optional Parameters")
   
    for _, v in pairs(tbl) do
        io.write("<div class=\"row\">\n")    
        self:printoption(v.name)
        io.write(v.description)
        io.write("</div>\n")
        
        io.write("<div class=\"row\">\n")    
        io.write(string.format("\n(Default value: %s)\n", 
            v.default))
        io.write("</div>\n")
   
    end
    io.write("<div class=\"row\"><br></div>\n")
end

function PG.inputs(self, sp)
    if sp[self.name].ninputs == 0 then return end;
    self:printheader("Inputs")
    local tbl = sp[self.name].inputs
    for _, v in pairs(tbl) do
        io.write("<div class=\"row\">\n")    
        self:printoption(v.name)
        io.write(v.description .. "\n")
        io.write("</div>\n")
    end
    io.write("<div class=\"row\"><br></div>\n")
end
    
function PG.outputs(self, sp)
    self:printheader("Outputs")
    local tbl = sp[self.name].outputs
    for _, v in pairs(tbl) do
    io.write("<div class=\"row\">\n")    
        self:printoption(v.name)
        io.write(v.description .. "\n")
    io.write("</div>\n")
    end
    io.write("<div class=\"row\"><br></div>\n")
end

function PG.other(self, sp)
    local tbl = sp[self.name].func.other
    if(tbl == nil) then return end    
    self:printheader("Other Functions:")
    for func,params in pairs(tbl) do
        io.write("<div class=\"row\">\n")
        io.write(string.format("%s(sp_data *sp, sp_%s *%s", func, self.name, self.name)) 
        for _,p in pairs(params.args) do
            io.write(string.format(", %s %s", p.type, p.name))
        end
        
        io.write(")\n")
        io.write("</div>\n")
        
        io.write("<div class=\"row\">\n")
        io.write(params.description)
        io.write("</div>\n")
        for _,p in pairs(params.args) do
            io.write("<div class=\"row\">\n")
            io.write("<div class=\"one column\"></div>\n")
            io.write("<div class=\"elevent columns\">\n")
            self:printoption(p.name)
            io.write(p.description.. "\n")
            io.write("</div></div>\n")
            
            io.write("<div class=\"row\">\n")
            io.write("<div class=\"one column\"></div>\n")
            io.write("<div class=\"elevent columns\">\n")
            io.write("(Suggested default: " .. p.default .. ")\n\n")
            io.write("</div></div>\n")
        end
    end
    io.write("<div class=\"row\"><br></div>\n")
end
    
function PG.params(self, sp)
    PG:man_params(sp)
    PG:opt_params(sp)
    io.write("\n")
end

function PG.files(self, sp)
    local tbl = sp[self.name].files
    io.write("<div class=\"row\">\n")
    io.write(string.format("Files: <i>%s, %s</i>", tbl.header, tbl.module))
    io.write("</div>\n")
    io.write("<div class=\"row\"><br></div>\n")
end

function PG.example(self, sp)
    local example = sp[self.name].files.example
    if (example == nil) then return end
    self:printheader("Example Code")
    file = io.open("examples/" .. example)
    io.write("<pre><code>")
    for line in file:lines() do
        io.write(string.gsub(line:gsub("<", "&lt;"), ">", "&gt;") .. "\n")
    end
    io.write("</code></pre>")
    file:close()
end

function PG.makepage(self, sp)
    io.write("<html>\n")
    io.write("<title>" .. self.name .. "</title>\n")
    io.write("<head>\n")
    io.write("<link rel=\"stylesheet\" href=\"css/skeleton.css\">\n")
    io.write("<link rel=\"stylesheet\" href=\"css/normalize.css\">\n")
    io.write("</head>\n")
    io.write("<body>\n")
    io.write("<div class=\"container\">\n")
    if(string.match(sptbl[self.name].modtype, "^module$")) then
        PG:title(sptbl)
        PG:files(sptbl)
        PG:desc(sptbl)
        PG:funcs(sptbl)
        PG:params(sptbl)
        PG:inputs(sptbl)
        PG:outputs(sptbl)
        PG:other(sptbl)
        PG:example(sptbl)
    end
    if(string.match(sptbl[self.name].modtype, "^gen$")) then
        PG:title(sptbl)
        PG:files(sptbl)
        PG:desc(sptbl)
        PG:genfuncs(sptbl)
        PG:genparams(sptbl)
        PG:example(sptbl)
    end
    io.write("</div>")
    io.write("</body>")
    io.write("</html>\n")
end

PG:makepage(sptbl)
