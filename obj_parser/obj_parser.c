/**************************************************
 *                  Galen Helfter
 *              ghelfter@clemson.edu
 *                   obj_parser.c
 **************************************************/

/* Define POSIX C for strtok_r use */
#define _POSIX_C_SOURCE 200809L

/* C standard library headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Local headers */
#include "obj_parser.h"

/* Set string comparisons as define statements */
#define OBJ_COMMENT_STR   "#"
#define OBJ_GROUP_STR     "g"
#define OBJ_MAT_STR       "mtllib"
#define OBJ_USEMAT_STR    "usemtl"
#define OBJ_GEOM_VERT_STR "v"
#define OBJ_TEXT_VERT_STR "vt"
#define OBJ_NORM_VERT_STR "vn"
#define OBJ_PARA_VERT_STR "vp"
#define OBJ_POINT_STR     "p"
#define OBJ_LINE_STR      "l"
#define OBJ_FACE_STR      "f"

/**************************************************
 *             Static function headers
 **************************************************/
static void copy_over_obj_struct(struct WavefrontOBJ *dest,
                                 const struct WavefrontOBJ *source);
static void obj_parse_vertex(char *ctxt, float *arr);
static void obj_parse_uvw(char *ctxt, float *arr);
static void obj_parse_norm(char *ctxt, float *arr);
static unsigned int obj_test_face_type(char *ctxt);
static void obj_read_face(char *ctxt_1, struct WavefrontOBJ *object);

/**************************************************
 *               External functions
 **************************************************/

void clear_struct(struct WavefrontOBJ *object)
{
    memset(object, 0x00, sizeof(struct WavefrontOBJ));
}

int parse_obj_file(const char *filename, struct WavefrontOBJ *object)
{
    int retcode = OBJ_PARSER_SUCCESS;
    /* Working obj file, we'll copy it to the input later */
    struct WavefrontOBJ working;

    /* Element indices */
    unsigned int c_l = 0u;

    char *line = NULL;
    char *tokenized = NULL;
    char *tok_res = NULL;
    size_t lsize = 0;
    int result = 0;
    int rval = 0;
    FILE *in_file = NULL;

    /* Initialize working to NULL and zero */
    clear_obj_struct(&working);

    /* Check for NULL being passed as either the object or filename */
    if(object == NULL || filename == NULL)
    {
        retcode = OBJ_PARSER_NULL_PTR;
        goto CLEANUP;
    }

    /* Clear our struct : Warning, will lose any allocated pointers, so
     * the struct should be clean */
    clear_struct(object);

    /* Open the file */
    in_file = fopen(filename, "r+");
    if(in_file == NULL)
    {
        retcode = OBJ_FOPEN_FAIL;
        goto CLEANUP;
    }

    /* Go through the file */
    while(!feof(in_file))
    {
        /* Get line from the file */
        rval = getline(&line, &lsize, in_file);
        if(rval < 0)
        {
            continue;
        }

        /* Check for comment */
        if(!strncmp(line, OBJ_COMMENT_STR, 1))
        {
            continue;
        }
        else if(!strncmp(line, OBJ_MAT_STR, 6))
        {
            continue;
        }
        else if(!strncmp(line, OBJ_GROUP_STR, 1))
        {
            continue;
        }
        else if(!strncmp(line, OBJ_USEMAT_STR, 6))
        {
            continue;
        }

        /* Go through the line and tokenize it */
        tok_res = strtok_r(line, " ", &tokenized);
        while(tok_res != NULL)
        {
            /* Check our vertex types */
            if(!strncmp(tok_res, OBJ_TEXT_VERT_STR, 2))
            {
                ++working.nuvs;
            }
            else if(!strncmp(tok_res, OBJ_NORM_VERT_STR, 2))
            {
                ++working.nnorms;
            }
            else if(!strncmp(tok_res, OBJ_PARA_VERT_STR, 2))
            {
                ++working.npara;
            }
            else if(!strncmp(tok_res, OBJ_GEOM_VERT_STR, 1))
            {
                ++working.nverts;
            }

            /* Now, check our element types */
            else if(!strncmp(tok_res, OBJ_FACE_STR, 1))
            {
                result = obj_test_face_type(tokenized);
                if(result == 3u) working.ntris += 1;
                else if(result == 4u) working.nquads += 1;
            }
            else if(!strncmp(tok_res, OBJ_POINT_STR, 1))
            {
            }
            else if(!strncmp(tok_res, OBJ_LINE_STR, 1))
            {
                ++working.nlines;
            }
            tok_res = strtok_r(NULL, " ", &tokenized);
        }
    }

    /* Allocate our structure memory*/
    if(working.nverts)
    {
        working.verts = malloc(sizeof(float) * 4 * working.nverts);
        working.nverts = 0u;
    }

    if(working.nuvs)
    {
        working.uvs = malloc(sizeof(float) * 2 * working.nuvs);
        working.nuvs = 0u;
    }
    if(working.nnorms)
    {
        working.norms = malloc(sizeof(float) * 3 * working.nnorms);
        working.nnorms = 0u;
    }

    if(working.ntris)
    {
        working.tris_verts = malloc(sizeof(unsigned int) * 3 * working.ntris);
        working.tris_uvs = malloc(sizeof(unsigned int) * 3 * working.ntris);
        working.tris_norms = malloc(sizeof(unsigned int) * 3 * working.ntris);
        working.ntris = 0u;
    }
    if(working.nquads)
    {
        working.quads_verts = malloc(sizeof(unsigned int) * 4 * working.nquads);
        working.quads_uvs = malloc(sizeof(unsigned int) * 4 * working.nquads);
        working.quads_norms = malloc(sizeof(unsigned int) * 4 * working.nquads);
        working.nquads = 0u;
    }
    if(working.nlines)
    {
        working.lines = malloc(sizeof(unsigned int) * 2 * working.nlines);
        working.nlines = 0u;
    }

    /* Reset back to the beginning of the file */
    fseek(in_file, 0, SEEK_SET);

    /* Go through our second pass of the file */
    while(!feof(in_file))
    {
        /* Get line from the file */
        rval = getline(&line, &lsize, in_file);

        if(rval < 0)
        {
            continue;
        }

        /* Check for comment */
        if(!strncmp(line, OBJ_COMMENT_STR, 1))
        {
            continue;
        }

        /* Go through the line and tokenize it */
        tok_res = strtok_r(line, " ", &tokenized);
        if(!strncmp(tok_res, OBJ_TEXT_VERT_STR, 2))
        {
            obj_parse_uvw(tokenized, working.uvs + working.nuvs*3u);
            working.nuvs += 1u;
        }
        else if(!strncmp(tok_res, OBJ_NORM_VERT_STR, 2))
        {
            obj_parse_norm(tokenized, working.norms + working.nnorms*3u);
            working.nnorms += 1u;
        }
        else if(!strncmp(tok_res, OBJ_PARA_VERT_STR, 2))
        {
        }
        else if(!strncmp(tok_res, OBJ_GEOM_VERT_STR, 1))
        {
            obj_parse_vertex(tokenized, working.verts + working.nverts*4u);
            working.nverts += 1u;
        }

        /* Check our element types */
        else if(!strncmp(tok_res, OBJ_FACE_STR, 1))
        {
            obj_read_face(tokenized, &working);
        }
        else if(!strncmp(tok_res, OBJ_LINE_STR, 1))
        {
            tok_res = strtok_r(NULL, " ", &tokenized);
            working.lines[c_l] = atol(tok_res);
            ++c_l;
            tok_res = strtok_r(NULL, " ", &tokenized);
            working.lines[c_l] = atol(tok_res);
            ++c_l;
        }
    }

CLEANUP:
    /* Clean up our file handle */
    if(in_file)
    {
        fclose(in_file);
    }
    in_file = NULL;

    /* Clean up our line pointer */
    if(line)
    {
        free(line);
    }
    line = NULL;

    if(retcode == OBJ_PARSER_SUCCESS)
    {
        copy_over_obj_struct(object, &working);
    }

    free_obj_file(&working);

    return retcode;
}

void obj_access_vertex(const struct WavefrontOBJ *object, float *x,
                       float *y, float *z, float *w, unsigned int ind)
{
    int index = ind * 4u;
    (*x) = *(object->verts + index + 0u);
    (*y) = *(object->verts + index + 1u);
    (*z) = *(object->verts + index + 2u);
    (*w) = *(object->verts + index + 3u);
}

void obj_access_uv(const struct WavefrontOBJ *object, float *u,
                   float *v, float *w, unsigned int ind)
{
    unsigned int index = ind * 3u;
    (*u) = *(object->uvs + index);
    (*v) = *(object->uvs + index + 1);
    (*w) = *(object->uvs + index + 2);
}

void obj_access_norm(const struct WavefrontOBJ *object, float *x,
                     float *y, float *z, unsigned int ind)
{
    unsigned int index = ind * 3;
    (*x) = *(object->norms + index);
    (*y) = *(object->norms + index + 1);
    (*z) = *(object->norms + index + 2);
}

void obj_access_tri_vert(const struct WavefrontOBJ *object, unsigned int *v1,
                         unsigned int *v2, unsigned int *v3, unsigned int ind)
{
    unsigned int lind = ind * 3u;
    (*v1) = *(object->tris_verts + lind + 0u);
    (*v2) = *(object->tris_verts + lind + 1u);
    (*v3) = *(object->tris_verts + lind + 2u);
}

void obj_access_tri_uvw(const struct WavefrontOBJ *object, unsigned int *v1,
                        unsigned int *v2, unsigned int *v3, unsigned int ind)
{
    unsigned int lind = ind * 3u;
    (*v1) = *(object->tris_uvs + lind + 0u);
    (*v2) = *(object->tris_uvs + lind + 1u);
    (*v3) = *(object->tris_uvs + lind + 2u);
}

void obj_access_tri_norm(const struct WavefrontOBJ *object, unsigned int *v1,
                         unsigned int *v2, unsigned int *v3, unsigned int ind)
{
    unsigned int lind = ind * 3u;
    (*v1) = *(object->tris_norms + lind + 0u);
    (*v2) = *(object->tris_norms + lind + 1u);
    (*v3) = *(object->tris_norms + lind + 2u);
}

/* Function to provide for clearing the object file struct */
void free_obj_file(struct WavefrontOBJ *object)
{
    if(object->verts != NULL) free(object->verts);
    object->verts = NULL;
    object->nverts = 0u;

    if(object->uvs != NULL) free(object->uvs);
    object->uvs = NULL;
    object->nuvs = 0u;

    if(object->norms != NULL) free(object->norms);
    object->norms = NULL;
    object->nnorms = 0u;

    if(object->lines != NULL) free(object->lines);
    object->lines = NULL;
    object->nlines = 0u;

    if(object->tris_verts != NULL) free(object->tris_verts);
    object->tris_verts = NULL;
    if(object->tris_uvs != NULL) free(object->tris_uvs);
    object->tris_uvs = NULL;
    if(object->tris_norms != NULL) free(object->tris_norms);
    object->tris_norms = NULL;
    object->ntris = 0u;

    /* Free our quad information */
    if(object->quads_verts != NULL) free(object->quads_verts);
    object->quads_verts = NULL;
    if(object->quads_uvs != NULL) free(object->quads_uvs);
    object->quads_uvs = NULL;
    if(object->quads_norms != NULL) free(object->quads_norms);
    object->quads_norms = NULL;
    object->nquads = 0u;
}

void clear_obj_struct(struct WavefrontOBJ *in_obj)
{
    if(in_obj)
    {
        in_obj->verts = in_obj->uvs = in_obj->norms = NULL;
        in_obj->lines = NULL;
        in_obj->tris_verts = in_obj->tris_uvs = in_obj->tris_norms = NULL;
        in_obj->quads_verts = in_obj->quads_uvs = in_obj->quads_norms = NULL;

        in_obj->nverts = in_obj->nuvs = in_obj->nnorms = 0u;
        in_obj->nlines = in_obj->npara = 0u;
        in_obj->ntris = in_obj->nquads = 0u;
    }
}


/**************************************************
 *             Static module functions
 **************************************************/
static void copy_over_obj_struct(struct WavefrontOBJ *dest,
                                 const struct WavefrontOBJ *source)
{
    if(dest != NULL && source != NULL)
    {
        dest->verts = source->verts;
        dest->uvs = source->uvs;
        dest->norms = source->norms;
        dest->lines = source->lines;
        dest->tris_verts = source->tris_verts;
        dest->tris_uvs = source->tris_uvs;
        dest->tris_norms = source->tris_norms;
        dest->quads_verts = source->quads_verts;
        dest->quads_uvs = source->quads_uvs;
        dest->quads_norms = source->quads_norms;

        dest->nverts = source->nverts;
        dest->nuvs = source->nuvs;
        dest->nnorms = source->nnorms;
        dest->nlines = source->nlines;
        dest->npara = source->npara;
        dest->ntris = source->ntris;
        dest->nquads = source->nquads;
    }
}

static void obj_parse_vertex(char *ctxt, float *arr)
{
    char *vsplit = NULL;
    float lvert[4] = {0.0, 0.0, 0.0, 1.0}; /* w coordinate defaults to 1.0 */
    unsigned int count = 0u, i = 0u;

    while((vsplit = strtok_r(NULL, " ", &ctxt)))
    {
        lvert[count] = strtof(vsplit, NULL);
        count += 1u;
    }

    for(i = 0u; i < 4u; ++i)
    {
        *(arr + i) = lvert[i];
    }
}

static void obj_parse_uvw(char *ctxt, float *arr)
{
    char *uvsplit = NULL;
    float luvw[3] = {0.0, 0.0, 0.0}; /* w coordinate defaults to 0.0 */
    unsigned int count = 0u, i = 0u;

    while((uvsplit = strtok_r(NULL, " ", &ctxt)))
    {
        luvw[count] = strtof(uvsplit, NULL);
        count += 1u;
    }

    for(i = 0u; i < 3u; ++i)
    {
        *(arr + i) = luvw[i];
    }
}

static void obj_parse_norm(char *ctxt, float *arr)
{
    char *nsplit = NULL;
    float lnorm[3] = {0.0, 0.0, 0.0};
    unsigned int count = 0u, i = 0u;

    while((nsplit = strtok_r(NULL, " ", &ctxt)))
    {
        lnorm[count] = strtof(nsplit, NULL);
        count += 1u;
    }

    for(i = 0u; i < 3u; ++i)
    {
        *(arr + i) = lnorm[i];
    }
}

static unsigned int obj_test_face_type(char *ctxt)
{
    char *fsplit = NULL;
    unsigned int count = 0u;
    while((fsplit = strtok_r(NULL, " ", &ctxt)))
    {
        count += 1u;
    }
    return count;
}

static void obj_read_face(char *ctxt_1, struct WavefrontOBJ *object)
{
    char *ctxt_2 = NULL;
    char *fsplit_1 = NULL;
    char *fsplit_2 = NULL;
    unsigned int count = 0u;
    unsigned int i = 0u;
    unsigned int verts[4] = {0u, 0u, 0u, 0u};
    unsigned int uvs[4] = {0u, 0u, 0u, 0u};
    unsigned int norms[4] = {0u, 0u, 0u, 0u};
    unsigned int *f_verts = NULL;
    unsigned int *f_uvs = NULL;
    unsigned int *f_norms = NULL;
    int wcode = 0;

    while((fsplit_1 = strtok_r(NULL, " ", &ctxt_1)))
    {
        if(strchr(fsplit_1, '\\') == NULL)
            wcode = 0;
        else if(strrchr(fsplit_1, '\\') == strchr(fsplit_1, '\\'))
            wcode = 1;
        else if(strrchr(fsplit_1, '\\') - strchr(fsplit_1, '\\') == 1)
            wcode = 2;
        else
            wcode = 3;
        if(wcode == 0)
        {
            verts[count] = (unsigned int) strtol(fsplit_1, NULL, 10) - 1;
        }
        else
        {
            fsplit_2 = strtok_r(fsplit_1, "/", &ctxt_2);
            if(wcode == 1)
            {
                uvs[count] = (unsigned int) strtol(fsplit_2, NULL, 10) - 1;
            }
            else if(wcode == 2)
            {
                norms[count] = (unsigned int) strtol(fsplit_2, NULL, 10) - 1;
            }
            else if(wcode == 3)
            {
                uvs[count] = (unsigned int) strtol(fsplit_2, NULL, 10) - 1;
                fsplit_2 = strtok_r(fsplit_1, "/", &ctxt_2);
                norms[count] = (unsigned int) strtol(fsplit_2, NULL, 10) - 1;
            }
        }

        count += 1u; /* Increment counter */
    }

    if(count == 3u)
    {
        f_verts = object->tris_verts + object->ntris * 3u;
        f_uvs = object->tris_uvs + object->ntris * 3u;
        f_norms = object->tris_norms + object->ntris * 3u;
        object->ntris += 1u;
    }
    else if(count == 4u)
    {
        f_verts = object->quads_verts + object->nquads * 4u;
        f_uvs = object->quads_uvs + object->nquads * 4u;
        f_norms = object->quads_norms + object->nquads * 4u;
        object->nquads += 1u;
    }
    else
    {
        return;
    }

    for(i = 0u; i < count; ++i)
    {
        *(f_verts + i) = verts[i];
        *(f_uvs + i) = uvs[i];
        *(f_norms + i) = norms[i];
    }
}
