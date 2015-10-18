proto_filetype = FileType([".proto"])

def gensrcjar_impl(ctx):
  out = ctx.outputs.srcjar
  proto_output = out.path + ".proto_output"
  proto_compiler = ctx.file._proto_compiler
  sub_commands = [
    "rm -rf " + proto_output,
    "mkdir " + proto_output,
    ' '.join([proto_compiler.path, "--java_out=" + proto_output,
              ctx.file.src.path]),
    "touch -t 198001010000 $(find " + proto_output + ")",
    ctx.file._jar.path + " cMf " + out.path + " -C " + proto_output + " .",
  ]

  ctx.action(
    command=" && ".join(sub_commands),
    inputs=[ctx.file.src, proto_compiler, ctx.file._jar] + ctx.files._jdk,
    outputs=[out],
    mnemonic="GenProtoSrcJar",
    use_default_shell_env = True)

gensrcjar = rule(
  gensrcjar_impl,
  attrs={
      "src": attr.label(allow_files=proto_filetype, single_file=True),
      # TODO(bazel-team): this should be a hidden attribute with a default
      # value, but Skylark needs to support select first.
      "_proto_compiler": attr.label(
          default=Label("//third_party:protoc"),
          allow_files=True,
          single_file=True),
      "_jar": attr.label(
          default=Label("//tools/jdk:jar"),
          allow_files=True,
          single_file=True),
      "_jdk": attr.label(
          default=Label("//tools/jdk:jdk"),
          allow_files=True),
  },
  outputs={"srcjar": "lib%{name}.srcjar"},
)

def proto_library(name, src, deps = None,
                  generate_cc = True,
                  generate_java = True):
  proto_cc_deps = [
      "//external:protoc",
      ]
  cc_deps = [
      "//external:protobuf_clib"
      ]
  command = "$(location //external:protoc) --cpp_out=$(GENDIR)"
  command += " $(location %s)" % (src)

  basename = src[0:-5]
  cc_proto_name = name + "_cc_proto"
  header_outputs = [
    basename + "pb.h",
  ]
  outputs = header_outputs + [
    basename + "pb.cc",
  ]
  native.genrule(
    name = cc_proto_name,
    srcs = [ src ] + proto_cc_deps,
    cmd = command,
    outs = outputs,
  )
  native.cc_library(
    name = name + "_cc",
    hdrs = header_outputs,
    srcs = [
      ":" + cc_proto_name
    ],
    deps = cc_deps,
  )
  gensrcjar(name=name + "_srcjar", src=src)
  native.java_library(
    name=name +"_java",
    srcs=[name + "_srcjar"],
    deps=["//third_party:protobuf"],
    # The generated code has lots of 'rawtypes' warnings.
    javacopts=["-Xlint:-rawtypes"],
  )
