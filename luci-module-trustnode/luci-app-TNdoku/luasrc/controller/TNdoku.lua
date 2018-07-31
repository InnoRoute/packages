--TrustNode statistics plugin
--ulbricht@innoroute.de 2018
module("luci.controller.TNdoku", package.seeall)
local http = require("luci.http")
function index()
    entry({"admin", "trustnode", "TNdoku"}, template("TNdoku"), _("TrustNode dokumentation"), 4)
--    entry({"admin", "trustnode", "TNstatistics"}, call("TNstatistics"))
end

