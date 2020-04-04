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
            path: "src",
            exclude: ["main.cpp"],
            publicHeadersPath: ".",
            cxxSettings: [
                .headerSearchPath("src")
            ]),
    ],
    cxxLanguageStandard: .cxx1z
)
