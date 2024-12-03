clean:
	@rm -rf Temp/bin/Release
	@rm -rf Temp/bin/Debug
	@rm -rf NugetOutput/*.nupkg

build: clean
	@dotnet build maestro-unity.csproj --configuration Release --no-incremental

pack: build
	@nuget pack maestro-unity.nuspec -OutputDirectory NugetOutput

