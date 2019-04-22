clean:
	@rm -rf Temp/bin/Release
	@rm -rf Temp/bin/Debug
	@rm -rf NugetOutput/*.nupkg

build: clean
	@msbuild maestro-unity.csproj /t:Rebuild /p:Configuration=Release

pack: build
	@nuget pack maestro-unity.nuspec -OutputDirectory NugetOutput

