#!/bin/bash

cd ./client/csharp

# see https://github.com/NuGet/Home/issues/7202 , using msbuild 14 for now.
nuget restore cs-pico.sln -MSBuildVersion 14

cd ../..

ls -al ./client/csharp/packages