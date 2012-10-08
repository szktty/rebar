#include <string.h>
#include "erl_nif.h"

typedef struct {{modid}}_nif_rsrc     {{modid}}_nif_rsrc;

struct {{modid}}_nif_rsrc {
    void *buf;
    size_t len;    
};

static int init(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info);
static int load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info);
static int upgrade(ErlNifEnv *env, void **priv_data, void **old_priv_data,
        ERL_NIF_TERM load_info);
static void unload(ErlNifEnv *env, void *priv_data);

static void {{modid}}_nif_cleanup_rsrc(ErlNifEnv *env, void *arg);

static ERL_NIF_TERM {{modid}}_nif_new(ErlNifEnv *env, int argc,
        const ERL_NIF_TERM argv[]);
static ERL_NIF_TERM {{modid}}_nif_get(ErlNifEnv *env, int argc,
        const ERL_NIF_TERM argv[]);

static ErlNifFunc nif_funcs[] =
{
    {"new", 1, {{modid}}_nif_new},
    {"get", 1, {{modid}}_nif_get},
};

ERL_NIF_INIT({{modid}}, nif_funcs, &load, NULL, &upgrade, &unload);

static ERL_NIF_TERM atom_ok;
static ERL_NIF_TERM atom_error;

static ErlNifResourceType *{{modid}}_nif_rsrc_type;

static int
init(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info)
{
    atom_ok = enif_make_atom(env, "ok");
    atom_error = enif_make_atom(env, "error");

    {{modid}}_nif_rsrc_type = enif_open_resource_type(env, NULL,
            "{{modid}}_nif_rsrc",
            &{{modid}}_nif_cleanup_rsrc,
            ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER, NULL);
    if ({{modid}}_nif_rsrc_type == NULL)
        return -1;

    return 0;
}

static int
load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info)
{
    return init(env, priv_data, load_info);
}

static int
upgrade(ErlNifEnv *env, void **priv_data, void **old_priv_data,
        ERL_NIF_TERM load_info)
{
    return init(env, priv_data, load_info);
}

static void
unload(ErlNifEnv *env, void *priv_data)
{
}

static void
{{modid}}_nif_cleanup_rsrc(ErlNifEnv *env, void *arg)
{
    {{modid}}_nif_rsrc *rsrc;

    rsrc = ({{modid}}_nif_rsrc *)arg;
    enif_free(rsrc->buf);
}

static ERL_NIF_TERM
{{modid}}_nif_new(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    ERL_NIF_TERM rsrc_term;
    {{modid}}_nif_rsrc *rsrc;
    ErlNifBinary bin;

    if (!enif_inspect_binary(env, argv[0], &bin))
        return 0;

    rsrc = enif_alloc_resource({{modid}}_nif_rsrc_type,
            sizeof({{modid}}_nif_rsrc));
    rsrc->buf = enif_alloc(bin.size);
    memcpy(rsrc->buf, bin.data, bin.size);
    rsrc->len = bin.size;
    rsrc_term = enif_make_resource(env, rsrc);
    enif_release_resource(rsrc);

    return rsrc_term;
}

static ERL_NIF_TERM
{{modid}}_nif_get(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    ERL_NIF_TERM bin_term;
    ErlNifBinary bin;
    {{modid}}_nif_rsrc *rsrc;

    if (!enif_get_resource(env, argv[0], {{modid}}_nif_rsrc_type,
                (void **)&rsrc))
        return 0;

    enif_alloc_binary(rsrc->len, &bin);
    memcpy(bin.data, rsrc->buf, rsrc->len);
    bin_term = enif_make_binary(env, &bin);
    enif_release_binary(&bin);

    return bin_term;
}

/* vim: set ts=4 sw=4 sts=4 tw=0: */

