// swift-tools-version:5.2

import PackageDescription

let package = Package(
    name: "Xylo",
    products: [
        .library(
            name: "Xylo",
            targets: ["Xylo"]),
    ],
    targets: [
        .target(
            name: "Xylo",
            path: ".",
            exclude: ["tests", "src/Main.cpp"],
            publicHeadersPath: "src",
            cxxSettings: [
                .headerSearchPath("include")
            ]),
    ],
    cxxLanguageStandard: .cxx1z
)
