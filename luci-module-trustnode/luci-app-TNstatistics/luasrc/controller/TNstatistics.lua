--TrustNode statistics plugin
--ulbricht@innoroute.de 2018	
module("luci.controller.TNstatistics", package.seeall)
local http = require("luci.http")
function index()
    entry({"admin", "trustnode", "TNstatistics"}, template("TNstatistics_rx-counters_dp0"), _("TrustNode statistics"), 4)
    	entry({"admin", "trustnode", "TNstatistics","Inport-Outport"}, template("TNstatistics_rx-counters_dp0"), _("Inport-Outport"), 0)
    		entry({"admin", "trustnode", "TNstatistics","Inport-Outport","Datapath 0"}, template("TNstatistics_rx-counters_dp0"), _("Datapath 0"), 0)
    		entry({"admin", "trustnode", "TNstatistics","Inport-Outport","Datapath 1"}, template("TNstatistics_rx-counters_dp1"), _("Datapath 1"), 1)
--    	entry({"admin", "trustnode", "TNstatistics","TX-counters"}, template("blank"), _("TX-counters"), 1)
--    		entry({"admin", "trustnode", "TNstatistics","TX-counters","Datapath 0"}, template("blank"), _("Datapath 0"), 0)
--    		entry({"admin", "trustnode", "TNstatistics","TX-counters","Datapath 1"}, template("blank"), _("Datapath 1"), 1)
    	entry({"admin", "trustnode", "TNstatistics","PHY and MAC states"}, template("blank"), _("PHY and MAC states"), 2)
    	entry({"admin", "trustnode", "TNstatistics","PHY counters"}, template("blank"), _("PHY counters"), 3)
    	entry({"admin", "trustnode", "TNstatistics","Drop and Bad counters"}, template("TNstatistics_drop_counters"), _("Drop and Bad counters"), 4)
    	--entry({"admin", "trustnode", "TNstatistics","Port counters"}, template("blank"), _("Port counters"), 5)
    	entry({"admin", "trustnode", "TNstatistics","Flow match counters"}, template("blank"), _("Flow match counters"), 6)
    	entry({"admin", "trustnode", "TNstatistics","Ethernet switch statistics"}, template("blank"), _("Ethernet switch statistics"), 7)
    
    entry({"admin", "trustnode", "TNstatisticsupdate"}, call("webcmd"))
end

function webcmd()
    local cmd = http.formvalue("cmd")
    if cmd then
	    local fp = io.popen("TNstatistics "..tostring(cmd).." -M 2>&1")
	    local result =  fp:read("*a")
	    fp:close()
        result = result:gsub("<", "&lt;")
        http.write(tostring(result))
    else
        http.write_json(http.formvalue())
    end
end
