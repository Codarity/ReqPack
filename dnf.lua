return {
	install = function(packages)
		local cmd = "dnf install -y"
		for _, package in ipairs(packages) do
			cmd = cmd .. " " .. package
		end
		os.execute(cmd)
		return true
	end,

	remove = function(packages)
		local cmd = "dnf remove -y"
		for _, package in ipairs(packages) do
			cmd = cmd .. " " .. package
		end
		os.execute(cmd)
		return true
	end,

	update = function()
		os.execute("dnf update -y")
		return true
	end,

	list = function()
		os.execute("dnf repoquery --userinstalled")
		return true
	end,

	search = function(term)
		local cmd = "dnf search " .. term
		os.execute(cmd)
		return true
	end,

	info = function(package)
		local cmd = "dnf info " .. package
		os.execute(cmd)
		return true
	end,

	exists = function(package)
		local handle = io.popen("dnf list installed " .. package .. " 2>/dev/null")
		local output = handle:read("*a")
		handle:close()
		return output:match(package) ~= nil
	end,

	check = function(package)
		local handle = io.popen("dnf check-update " .. package .. " 2>/dev/null")
		local output = handle:read("*a")
		handle:close()
		return output ~= "" and not output:match("No packages marked for update")
	end
}
