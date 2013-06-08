lib_name = "swag"
dependencies = {  }

solution (lib_name)
	configurations { "Debug", "Release" }

	project (lib_name)
		kind "StaticLib"
		language "C++"
		location "build"
		files { "src/*.cpp" }
		targetdir "build/lib"
		includedirs { "include" }
		buildoptions { "-fPIC", "-m64", "-std=c++11" }

		configuration { "linux", "gmake" }
			buildoptions { "-pg" }
			linkoptions { "-pg" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
 
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

	project ("swag_hc")
		kind "StaticLib"
		language "C++"
		location "build"
		files { "hardcoded_views/*.cpp" }
		targetdir "build/lib"
		includedirs { "src" }
		buildoptions { "-fPIC", "-m64", "-std=c++11" }

		configuration { "linux", "gmake" }
			buildoptions { "-pg" }
			linkoptions { "-pg" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
 
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

	project ("layout_editor")
		kind "ConsoleApp"
		language "C++"
		location "build"
		files { "layout_editor/*.cpp" }
		includedirs { "src", "hardcoded_views" }
		libdirs { "build/lib" }
		links ("swag_hc")
		links (lib_name)
		links {"allegro","allegro_main","allegro_image","allegro_font","allegro_ttf", "allegro_primitives","allegro_dialog", "yaml-cpp" }
		targetdir "build/layout_editor"
		configuration { "linux", "gmake" }
			buildoptions { "-pg" }
			linkoptions { "-pg" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
 
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

	ex_dependencies = {"allegro","allegro_main","allegro_image","allegro_font","allegro_ttf",
	"allegro_primitives","allegro_dialog", "yaml-cpp" }
	examples = os.matchfiles("examples/*.cpp")
	for index, name in pairs(examples) do
		sname = name:sub(10, name:len()-4);
		project (sname)
			kind "ConsoleApp"
			language "C++"
			location "build"
			files { name }
			includedirs { "src", "hardcoded_views" }
			libdirs { "build/lib" }
			links ("swag_hc")
			links (lib_name)
			links (ex_dependencies)
			targetdir "build/examples"
--			postbuildcommands { "cd .. && build/examples/"..sname }
			configuration { "linux", "gmake" }
				buildoptions { "-pg" }
				linkoptions { "-pg" }

			configuration "Debug"
				defines { "DEBUG" }
				flags { "Symbols" }
	 
			configuration "Release"
				defines { "NDEBUG" }
				flags { "Optimize" }
	end

newoption {
   trigger     = "dir",
   value       = "path",
   description = "Choose a path to install dir",
}

newaction {
	trigger     = "install",
	description = "Install the software",
	execute = function ()
		-- copy files, etc. here
		os.mkdir(_OPTIONS["dir"].."lib/");
		files = os.matchfiles("build/lib/*")
		print ("Installing lib files to " .. _OPTIONS["dir"] .."lib/")
		for k, f in pairs(files) do
			print ("Copying " .. f)
			os.copyfile(f, _OPTIONS["dir"].."lib/")
		end
		os.mkdir(_OPTIONS["dir"].."include/"..lib_name.."/");
		files = os.matchfiles("src/*.h")
		print ("Installing header files to " .. _OPTIONS["dir"] .."include/")
		for k, f in pairs(files) do
			print ("Copying " .. f)
			os.copyfile(f, _OPTIONS["dir"].."include/"..lib_name.."/")
		end
	end
}

if not _OPTIONS["dir"] then
   _OPTIONS["dir"] = "/usr/local/"
end

if not ("/" == _OPTIONS["dir"]:sub(_OPTIONS["dir"]:len())) then
	_OPTIONS["dir"] = _OPTIONS["dir"] .. "/"
end
