// swift-tools-version:5.2

import PackageDescription

let package = Package(
    name: "Xylo",
    platforms: [
        .macOS(.v10_14), .iOS(.v12), .tvOS(.v12), .watchOS(.v5)
    ],
    products: [
        .library(
            name: "Xylo",
            targets: ["Xylo"]),
    ],
    targets: [
        .target(
            name: "Xylo",
            path: ".",
            exclude: ["tests",
                      "src/main.cpp",
                      "src/CMakeLists.txt",
                      "src/ast/CMakeLists.txt",
                      "src/codegen/CMakeLists.txt",
                      "src/native/CMakeLists.txt",
                      "src/parser/CMakeLists.txt",
                      "src/token/CMakeLists.txt",
                      "src/vm/CMakeLists.txt"],
            sources: ["src"],
            publicHeadersPath: "include",
            cxxSettings: [
                .headerSearchPath("include")
            ]),
    ],
    cxxLanguageStandard: .cxx1z
)
