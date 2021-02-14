const fs = require('fs')
const os = require('os')
const path = require('path')

function cmake(a) {
  let extra = [];

  let userPackageLocal = path.join(os.homedir(), ".config/CPM/package-local.cmake");
  if(fs.existsSync(userPackageLocal)) {
    extra.push(`-C ${userPackageLocal}`);
  }

  return { options: { args: `${extra.join(' ')} -DTOOLCHAIN=${a.tc} -DBUILD_CONFIG=${a.c}` } };
}

module.exports = function(grunt) {
  grunt.loadNpmTasks('grunt-rpav-cmake');
  grunt.loadNpmTasks('grunt-run');

  grunt.initConfig({
    cmake_config: {
      options: {
        buildDir: o => "build/" + o.buildConfig,
        env: [
          ["LSAN_OPTIONS", "suppressions=../../../lsan_suppress.txt"],
        ],
      },

      "clang-Debug": cmake({tc: 'clang', c: 'Debug'}),
      "clang-Sanitize": cmake({tc: 'clang', c: 'Sanitize'}),
      "clang-Release": cmake({tc: 'clang', c: 'Release'}),

      "gcc-Debug": cmake({tc: 'gcc', c: 'Debug'}),
      "gcc-Sanitize": cmake({tc: 'gcc', c: 'Sanitize'}),
      "gcc-Release": cmake({tc: 'gcc', c: 'Release'}),
    },

    cmake_run: {
      all: { build: "all" },
      test: { build: "test" },
    },

    cmake_build_run: {
      all: {},
      test: {},
    },

    run: {},
  });
};
