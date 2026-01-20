# Release Guide

This document describes the step-by-step process to build, package, and publish the `maestro_unity` NuGet package to JFrog Artifactory.

## Prerequisites

Before releasing, ensure you have the following installed:

- **CMake** (>= 3.2)
- **Conan** (>= 1.53.0)
- **Docker** (for building Linux libraries)
- **Mono/.NET SDK** with `msbuild`
- **NuGet CLI** (`nuget`)
- **Boost 1.63.0** (see main README for installation)

## Configuration

### 1. Check JFrog Artifactory Setup Instructions

> ⚠️ **Important**: The `maestro_unity` package is hosted in JFrog Artifactory under the `npm-local` repository (`com.wildlifestudios.nuget.maestro_unity`). Despite being a NuGet package, the repository type may require specific configuration.
>
> **Before proceeding, navigate to the JFrog Artifactory UI:**
> 1. Go to Wildlife Studios JFrog Artifactory
> 2. Navigate to **Artifactory** → **Artifacts** → **npm-local**
> 3. Click on **"Set Me Up"** button in the top right
> 4. Follow the exact instructions provided for your repository type
>
> The setup instructions will tell you whether you need to configure:
> - **NuGet source** (using `nuget sources Add`)
> - **NPM registry** (using `.npmrc` file)
> - Or a combination of both

### 2. Option A: NuGet Source Configuration

If the repository accepts NuGet packages directly, add the JFrog Artifactory as a NuGet source:

```bash
# Add the JFrog NuGet repository
nuget sources Add \
  -Name "WildlifeJFrog" \
  -Source "https://wildlifestudios.jfrog.io/artifactory/api/nuget/v3/npm-local" \
  -Username "<your-jfrog-username>" \
  -Password "<your-jfrog-api-key>"

# Set API key for the source
nuget setapikey <your-jfrog-api-key> -Source "WildlifeJFrog"
```

Verify the configuration:

```bash
nuget sources list
```

### 3. Option B: NPM Registry Configuration (.npmrc)

If the `npm-local` repository requires NPM-style authentication, configure your `.npmrc`:

```bash
# Create or edit ~/.npmrc (or project-level .npmrc)
cat >> ~/.npmrc << 'EOF'
@wildlifestudios:registry=https://wildlifestudios.jfrog.io/artifactory/api/npm/npm-local/
//wildlifestudios.jfrog.io/artifactory/api/npm/npm-local/:_authToken=<your-jfrog-api-key>
//wildlifestudios.jfrog.io/artifactory/api/npm/npm-local/:always-auth=true
EOF
```

Or use the JFrog CLI:

```bash
# Login to JFrog (follow prompts)
jfrog rt c

# Configure npm
jfrog rt npmc --repo-resolve=npm-local --repo-deploy=npm-local
```

> **Note**: You can generate an API key from JFrog Artifactory UI under **User Profile** → **Edit Profile** → **Generate API Key** (or **Generate Identity Token**).

## Release Process

### Step 1: Update Version Numbers

Update the version in **both** files:

1. **`maestro-unity/maestro-unity.nuspec`**:
   ```xml
   <version>X.Y.Z</version>
   ```

2. **`maestro-unity/maestro-unity.csproj`**:
   ```xml
   <PackageVersion>X.Y.Z</PackageVersion>
   ```

Also update the `<releaseNotes>` in the nuspec file to describe what changed:
```xml
<releaseNotes>Brief description of changes</releaseNotes>
```

### Step 2: Build Native Libraries

Build the native libraries for both macOS and Linux:

**Option A: Using the build script (recommended)**

```bash
./build_libs.sh
```

This script will:
- Build `libmaestro.bundle` for macOS (if running on macOS)
- Build `libmaestro.so` for Linux using Docker

**Option B: Using Makefile targets**

```bash
# Build everything
make build-all

# Or build individually:
make build-mac-unity      # macOS .bundle
make build-linux-docker   # Linux .so via Docker
```

### Step 3: Verify Native Libraries

Confirm the native libraries were built successfully:

```bash
ls -la _builds/mac-unity/lib/libmaestro.bundle
ls -la _builds/linux/lib/libmaestro.so
```

### Step 4: Build and Pack NuGet Package

Navigate to the Unity package directory and build:

```bash
cd maestro-unity
make pack
```

This command will:
1. Clean previous builds
2. Build the C# project with `msbuild`
3. Create the NuGet package with `nuget pack`

The `.nupkg` file will be created at `maestro-unity/NugetOutput/maestro_unity.X.Y.Z.nupkg`.

### Step 5: Verify Package Contents

Before publishing, verify the package contents:

```bash
# List package contents
nuget verify -Signatures NugetOutput/maestro_unity.*.nupkg

# Or unzip and inspect manually
unzip -l NugetOutput/maestro_unity.*.nupkg
```

Expected contents:
- `Native/libmaestro.bundle` (macOS)
- `Native/libmaestro.bundle.meta`
- `Native/libmaestro.so` (Linux)
- `Native/libmaestro.so.meta`
- `lib/net35/maestro-unity.dll`
- `lib/net35/maestro-unity.pdb`

### Step 6: Publish to JFrog Artifactory

> ⚠️ **Important**: Check the exact publish command in JFrog Artifactory's **"Set Me Up"** dialog for the `npm-local` repository. The command below may vary based on your repository configuration.

**Option A: Using NuGet CLI**

```bash
nuget push NugetOutput/maestro_unity.X.Y.Z.nupkg -Source "WildlifeJFrog"
```

Or with inline credentials:

```bash
nuget push NugetOutput/maestro_unity.X.Y.Z.nupkg \
  -Source "https://wildlifestudios.jfrog.io/artifactory/api/nuget/v3/npm-local" \
  -ApiKey "<your-jfrog-api-key>"
```

**Option B: Using JFrog CLI**

```bash
jfrog rt upload "NugetOutput/maestro_unity.X.Y.Z.nupkg" "npm-local/"
```

**Option C: Using cURL (direct upload)**

```bash
curl -u "<username>:<api-key>" \
  -X PUT \
  "https://wildlifestudios.jfrog.io/artifactory/npm-local/maestro_unity.X.Y.Z.nupkg" \
  -T NugetOutput/maestro_unity.X.Y.Z.nupkg
```

Replace `X.Y.Z` with the actual version number.

### Step 7: Commit and Tag Release

After successful publication:

```bash
# Commit version bump
git add maestro-unity/maestro-unity.nuspec maestro-unity/maestro-unity.csproj
git commit -m "Bump maestro-unity to vX.Y.Z"

# Create git tag (optional but recommended)
git tag -a vX.Y.Z -m "Release vX.Y.Z"

# Push changes
git push origin main
git push origin vX.Y.Z
```

## Troubleshooting

### Authentication Issues

If you encounter authentication errors:

1. Verify your API key is valid in JFrog UI
2. Check the **"Set Me Up"** dialog in JFrog Artifactory for the correct authentication method
3. Re-add the source with correct credentials:
   ```bash
   nuget sources Remove -Name "WildlifeJFrog"
   nuget sources Add -Name "WildlifeJFrog" -Source "..." -Username "..." -Password "..."
   ```

### NPM-local Repository Issues

Since this is an `npm-local` repository hosting a NuGet package:

1. The repository may require NPM-style authentication (`.npmrc`) instead of NuGet sources
2. Check if the repository accepts `.nupkg` files directly or requires a different format
3. Contact your JFrog administrator if unsure about the correct upload method
4. Try using the JFrog CLI or direct cURL upload as alternatives

### Build Failures

**Linux build fails in Docker:**
```bash
# Rebuild the Docker image
docker build -f Dockerfile-builder -t maestro-example-builder:latest .
```

**macOS build fails:**
- Ensure Boost is installed at `/usr/local/boost_1_63_0`
- Verify `BOOST_ROOT` environment variable is set

### Package Already Exists

If the version already exists in Artifactory, you must either:
1. Delete the existing version from Artifactory (if allowed)
2. Increment the version number

## Quick Reference

```bash
# Full release process (after updating version numbers)
./build_libs.sh
cd maestro-unity && make pack

# Publish (choose one based on your JFrog setup - check "Set Me Up" in Artifactory)
nuget push NugetOutput/maestro_unity.X.Y.Z.nupkg -Source "WildlifeJFrog"
# OR: jfrog rt upload "NugetOutput/maestro_unity.X.Y.Z.nupkg" "npm-local/"

git add -A && git commit -m "Bump maestro-unity to vX.Y.Z"
git tag -a vX.Y.Z -m "Release vX.Y.Z"
git push origin main --tags
```

## Package Information

- **Package ID**: `maestro_unity`
- **JFrog Repository**: `npm-local`
- **Full Package Name**: `com.wildlifestudios.nuget.maestro_unity`
- **Target Framework**: .NET 3.5 (Unity compatible)
- **JFrog Artifactory URL**: https://wildlifestudios.jfrog.io

> 📝 **Note**: This package is a NuGet package (`.nupkg`) hosted in an `npm-local` repository. Always refer to JFrog Artifactory's **"Set Me Up"** instructions for the most accurate configuration and publish commands.
