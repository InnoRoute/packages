--TrustNode statistics plugin
--ulbricht@innoroute.de 2018	
module("luci.controller.TNstatistics", package.seeall)
local http = require("luci.http")
function index()
    entry({"admin", "trustnode", "TNstatistics"}, template("TNstatistics"), _("TrustNode statistics"), 4)
    entry({"admin", "trustnode", "TNstatisticsupdate"}, call("webcmd"))
end

function webcmd()
    local cmd = http.formvalue("cmd")
    if cmd then
	    local fp = io.popen(tostring(cmd).." 2>&1")
	    local result =  fp:read("*a")
	    fp:close()
        result = result:gsub("<", "&lt;")
        http.write(tostring(result))
    else
        http.write_json(http.formvalue())
    end
end
