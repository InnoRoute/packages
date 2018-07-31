--TrustNode bitstream upload
--adapted from https://github.com/seamustuohy/luci_tutorials/blob/master/examples/uploader
--ulbricht@innoroute.de 2018

module("luci.controller.TNupload", package.seeall)
local http = require("luci.http")
function index()
    entry({"admin", "trustnode", "TNupload"}, template("TNupload"), _("Bitstream upload"), 4)
    entry({"admin", "trustnode", "TNuploading"}, call("uploader"))
end

function uploader()
   local error = nil	
   --add your defaults (file_name is optional btw)
   file_loc = "/boot/"
   input_field = "input-name"
   file_name = "trustnode_top.bit"
   --actually call the file handler
   --get the values from the forms on the page
   local values = luci.http.formvalue()
   --get the value of the input field
   local ul = values[input_field]
   --make sure something is being uploaded
   if ul ~= '' and ul ~= nil then
	  --Start your uploader
	  setFileHandler(file_loc, input_field, file_name)
	  local fp = io.popen(tostring("TNchangemod 8").." 2>&1")
	  local result =  fp:read("*a")
	  fp:close()
              
   end
--   luci.template.render("TNupload", {error=error})
end

--setFileHandler (stolen from sysUpgrade)
--location: (string) The full path to where the file should be saved.
--input_name: (string) The name specified by the input html field.
--<input type="submit" name="input_name_here" value="whatever you want"/>
--file_name: (string, optional) The optional name you would like the file to be saved as. If left blank the file keeps its uploaded name.
function setFileHandler(location, input_name, file_name)
	  local sys = require "luci.sys"
--	  local fs = require "luci.fs"
	  local fp
	  luci.http.setfilehandler(
		 function(meta, chunk, eof)
		 if not fp then
			--make sure the field name is the one we want
			if meta and meta.name == input_name then
			   --use the file name if specified
			   if file_name ~= nil then
				  fp = io.open(location .. file_name, "w")
			   else
				  fp = io.open(location .. meta.file, "w")
			   end
			end
		 end
		 --actually do the uploading.
		 if chunk then
			fp:write(chunk)
		 end
		 if eof then
			fp:close()
		 end
		 end)
end
