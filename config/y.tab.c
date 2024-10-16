/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 15 "config.y"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Avoid warnings for undeclared items */

extern	char	*yytext;
extern	int	yyleng;
extern	int	yylex(void);


/********************************************************************************/
/*										*/
/*			Start of Definitions					*/
/*										*/
/********************************************************************************/

#define	NIL	(struct dev_ent *)0x00

#define	CONFC	 "conf.c"		/* Name of .c output			*/
#define CONFH	 "conf.h"		/* Name of .h output			*/
#define	CONFHREF "<conf.h>"		/* How conf.h referenced		*/
#define	INFILE	 "Configuration"	/* Name of input file			*/
#define	MAXNAME  16			/* Max length of names			*/

#define	NDEVS		250		/* Max devices				*/
#define	NTYPES		250		/* Max device types			*/

int	linectr = 1;

FILE	*confc;
FILE	*confh;

int	brkcount = 0;			/* Count of %% separators till now in	*/
					/*  the input file			*/
char	*doing = "device type declarations";

struct	dev_ent	{			/* Entry for a device or device type	*/
	char	name[MAXNAME];		/* device name (unused in a type)	*/
	char	tname[MAXNAME];		/* Type name				*/
	char	ison[MAXNAME];		/* Name is "on" XXX			*/
	int	tindex;			/* Index in dtypes (unused in a type)	*/
	int	csr;			/* Control Status Register addr		*/
	int	irq;			/* interrupt request			*/
	char	intr[MAXNAME];		/* interrupt function name		*/
	char	init[MAXNAME];		/* init function name			*/
	char	open[MAXNAME];		/* open function name			*/
	char	close[MAXNAME];		/* close function name			*/
	char	read[MAXNAME];		/* read function name			*/
	char	write[MAXNAME];		/* write function name			*/
	char	control[MAXNAME];	/* control function name		*/
	char	seek[MAXNAME];		/* seek function name			*/
	char	getc[MAXNAME];		/* getc function name			*/
	char	putc[MAXNAME];		/* putc function name			*/
	int	minor;			/* In a device, the minor device	*/
					/*  assigned to the device 0,1,...	*/
					/*  in a type, the next minor number	*/
					/*  to assign				*/
};
struct	dev_ent		dtypes[NTYPES];/* Table of all device types		*/
int	ntypes	= 0;			/* Number of device types found		*/

struct	dev_ent		devs[NDEVS];	/* Table of all devices			*/
int	ndevs = 0;			/* Number of devices found		*/

char *devstab[] = {
	"/* Device table entry */",
	"struct\tdentry\t{",
	"\tint32   dvnum;",
	"\tint32   dvminor;",
	"\tchar    *dvname;",
	"\tdevcall (*dvinit) (struct dentry *);",
	"\tdevcall (*dvopen) (struct dentry *, char *, char *);",
	"\tdevcall (*dvclose)(struct dentry *);",
	"\tdevcall (*dvread) (struct dentry *, void *, uint32);",
	"\tdevcall (*dvwrite)(struct dentry *, void *, uint32);",
	"\tdevcall (*dvseek) (struct dentry *, int32);",
	"\tdevcall (*dvgetc) (struct dentry *);",
	"\tdevcall (*dvputc) (struct dentry *, char);",
	"\tdevcall (*dvcntl) (struct dentry *, int32, int32, int32);",
	"\tvoid    *dvcsr;",
	"\tvoid    (*dvintr)(void);",
	"\tbyte    dvirq;",
	"};\n",
	"extern	struct	dentry	devtab[]; /* one entry per device */",
	NULL
};

char	saveattrid[MAXNAME];		/* Holds the IDENT from an attribute	*/

/********************************************************************************/
/*										*/
/*			       Function prototypes				*/
/*										*/
/********************************************************************************/

void	addattr(int, int);
int	addton(char *);
int	config_atoi(char *, int);
void	devisid(char *);
void	devonid(char *);
void	getattrid(char *);
void	newdev(char *);
int	newtype(char *);
void	yyerror(char *);



#line 182 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    DEFBRK = 258,                  /* DEFBRK  */
    IFBRK = 259,                   /* IFBRK  */
    COLON = 260,                   /* COLON  */
    OCTAL = 261,                   /* OCTAL  */
    INTEGER = 262,                 /* INTEGER  */
    IDENT = 263,                   /* IDENT  */
    CSR = 264,                     /* CSR  */
    IRQ = 265,                     /* IRQ  */
    INTR = 266,                    /* INTR  */
    INIT = 267,                    /* INIT  */
    OPEN = 268,                    /* OPEN  */
    CLOSE = 269,                   /* CLOSE  */
    READ = 270,                    /* READ  */
    WRITE = 271,                   /* WRITE  */
    SEEK = 272,                    /* SEEK  */
    CONTROL = 273,                 /* CONTROL  */
    IS = 274,                      /* IS  */
    ON = 275,                      /* ON  */
    GETC = 276,                    /* GETC  */
    PUTC = 277                     /* PUTC  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define DEFBRK 258
#define IFBRK 259
#define COLON 260
#define OCTAL 261
#define INTEGER 262
#define IDENT 263
#define CSR 264
#define IRQ 265
#define INTR 266
#define INIT 267
#define OPEN 268
#define CLOSE 269
#define READ 270
#define WRITE 271
#define SEEK 272
#define CONTROL 273
#define IS 274
#define ON 275
#define GETC 276
#define PUTC 277

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);



/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DEFBRK = 3,                     /* DEFBRK  */
  YYSYMBOL_IFBRK = 4,                      /* IFBRK  */
  YYSYMBOL_COLON = 5,                      /* COLON  */
  YYSYMBOL_OCTAL = 6,                      /* OCTAL  */
  YYSYMBOL_INTEGER = 7,                    /* INTEGER  */
  YYSYMBOL_IDENT = 8,                      /* IDENT  */
  YYSYMBOL_CSR = 9,                        /* CSR  */
  YYSYMBOL_IRQ = 10,                       /* IRQ  */
  YYSYMBOL_INTR = 11,                      /* INTR  */
  YYSYMBOL_INIT = 12,                      /* INIT  */
  YYSYMBOL_OPEN = 13,                      /* OPEN  */
  YYSYMBOL_CLOSE = 14,                     /* CLOSE  */
  YYSYMBOL_READ = 15,                      /* READ  */
  YYSYMBOL_WRITE = 16,                     /* WRITE  */
  YYSYMBOL_SEEK = 17,                      /* SEEK  */
  YYSYMBOL_CONTROL = 18,                   /* CONTROL  */
  YYSYMBOL_IS = 19,                        /* IS  */
  YYSYMBOL_ON = 20,                        /* ON  */
  YYSYMBOL_GETC = 21,                      /* GETC  */
  YYSYMBOL_PUTC = 22,                      /* PUTC  */
  YYSYMBOL_YYACCEPT = 23,                  /* $accept  */
  YYSYMBOL_configuration = 24,             /* configuration  */
  YYSYMBOL_devtypes = 25,                  /* devtypes  */
  YYSYMBOL_devtype = 26,                   /* devtype  */
  YYSYMBOL_tname = 27,                     /* tname  */
  YYSYMBOL_dev_tlist = 28,                 /* dev_tlist  */
  YYSYMBOL_theader = 29,                   /* theader  */
  YYSYMBOL_tonid = 30,                     /* tonid  */
  YYSYMBOL_attr_list = 31,                 /* attr_list  */
  YYSYMBOL_attr = 32,                      /* attr  */
  YYSYMBOL_id = 33,                        /* id  */
  YYSYMBOL_number = 34,                    /* number  */
  YYSYMBOL_devices = 35,                   /* devices  */
  YYSYMBOL_device = 36,                    /* device  */
  YYSYMBOL_dheader = 37,                   /* dheader  */
  YYSYMBOL_dname = 38,                     /* dname  */
  YYSYMBOL_devis = 39,                     /* devis  */
  YYSYMBOL_devisid = 40,                   /* devisid  */
  YYSYMBOL_devon = 41,                     /* devon  */
  YYSYMBOL_devonid = 42                    /* devonid  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   39

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  23
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  35
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  58

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   277


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   133,   133,   142,   143,   146,   149,   152,   153,   156,
     159,   162,   163,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   180,   183,   193,   194,   197,
     200,   203,   206,   209,   212,   215
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DEFBRK", "IFBRK",
  "COLON", "OCTAL", "INTEGER", "IDENT", "CSR", "IRQ", "INTR", "INIT",
  "OPEN", "CLOSE", "READ", "WRITE", "SEEK", "CONTROL", "IS", "ON", "GETC",
  "PUTC", "$accept", "configuration", "devtypes", "devtype", "tname",
  "dev_tlist", "theader", "tonid", "attr_list", "attr", "id", "number",
  "devices", "device", "dheader", "dname", "devis", "devisid", "devon",
  "devonid", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-14)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -14,    10,    22,   -14,   -14,   -14,   -14,     6,    16,     8,
     -14,   -14,   -14,    12,    18,     8,   -14,    -9,    19,    13,
     -14,   -14,   -14,    -9,    25,    25,    26,    26,    26,    26,
      26,    26,    26,    26,    26,    26,   -14,   -14,   -14,    27,
     -14,    -9,   -14,   -14,   -14,   -14,   -14,   -14,   -14,   -14,
     -14,   -14,   -14,   -14,   -14,   -14,   -14,   -14
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     1,    27,     6,     4,     0,     2,     0,
      31,    28,    11,     0,     0,     5,    11,    29,     0,     0,
      10,     9,    11,     7,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    12,    33,    32,     0,
      30,     8,    26,    13,    14,    25,    15,    18,    16,    17,
      21,    22,    23,    24,    19,    20,    35,    34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -14,   -14,   -14,   -14,   -14,   -14,    21,   -14,     7,   -14,
     -13,    14,   -14,   -14,   -14,   -14,   -14,   -14,   -14,   -14
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     6,     7,    15,    16,    21,    17,    36,
      46,    43,     8,    11,    12,    13,    19,    38,    40,    57
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
       3,     9,    34,    35,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    23,    10,     4,    20,    37,    14,    41,
       5,    18,    42,    39,    45,    56,    22,     0,     0,    44
};

static const yytype_int8 yycheck[] =
{
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
       0,     5,    21,    22,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    16,     8,     3,     8,     8,    20,    22,
       8,    19,     7,    20,     8,     8,    15,    -1,    -1,    25
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    24,    25,     0,     3,     8,    26,    27,    35,     5,
       8,    36,    37,    38,    20,    28,    29,    31,    19,    39,
       8,    30,    29,    31,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    21,    22,    32,     8,    40,    20,
      41,    31,     7,    34,    34,     8,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,     8,    42
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    23,    24,    25,    25,    26,    27,    28,    28,    29,
      30,    31,    31,    32,    32,    32,    32,    32,    32,    32,
      32,    32,    32,    32,    32,    33,    34,    35,    35,    36,
      37,    38,    39,    40,    41,    42
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     0,     2,     3,     1,     2,     3,     2,
       1,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     1,     0,     2,     2,
       3,     1,     2,     1,     2,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* devtypes: %empty  */
#line 142 "config.y"
                              { doing = "device definitions"; }
#line 1317 "y.tab.c"
    break;

  case 6: /* tname: IDENT  */
#line 149 "config.y"
                      { yyval = newtype(yytext); }
#line 1323 "y.tab.c"
    break;

  case 9: /* theader: ON tonid  */
#line 156 "config.y"
                         { yyval = yyvsp[0]; }
#line 1329 "y.tab.c"
    break;

  case 10: /* tonid: IDENT  */
#line 159 "config.y"
                      { yyval = addton(yytext); }
#line 1335 "y.tab.c"
    break;

  case 13: /* attr: CSR number  */
#line 166 "config.y"
                                        { addattr(CSR,    yyvsp[0]);	}
#line 1341 "y.tab.c"
    break;

  case 14: /* attr: IRQ number  */
#line 167 "config.y"
                                        { addattr(IRQ,    yyvsp[0]);	}
#line 1347 "y.tab.c"
    break;

  case 15: /* attr: INTR id  */
#line 168 "config.y"
                                        { addattr(INTR,    0);	}
#line 1353 "y.tab.c"
    break;

  case 16: /* attr: OPEN id  */
#line 169 "config.y"
                                        { addattr(OPEN,    0);	}
#line 1359 "y.tab.c"
    break;

  case 17: /* attr: CLOSE id  */
#line 170 "config.y"
                                        { addattr(CLOSE,   0);	}
#line 1365 "y.tab.c"
    break;

  case 18: /* attr: INIT id  */
#line 171 "config.y"
                                        { addattr(INIT,    0);	}
#line 1371 "y.tab.c"
    break;

  case 19: /* attr: GETC id  */
#line 172 "config.y"
                                        { addattr(GETC,    0);	}
#line 1377 "y.tab.c"
    break;

  case 20: /* attr: PUTC id  */
#line 173 "config.y"
                                        { addattr(PUTC,    0);	}
#line 1383 "y.tab.c"
    break;

  case 21: /* attr: READ id  */
#line 174 "config.y"
                                        { addattr(READ,    0);	}
#line 1389 "y.tab.c"
    break;

  case 22: /* attr: WRITE id  */
#line 175 "config.y"
                                        { addattr(WRITE,   0);	}
#line 1395 "y.tab.c"
    break;

  case 23: /* attr: SEEK id  */
#line 176 "config.y"
                                        { addattr(SEEK,    0);	}
#line 1401 "y.tab.c"
    break;

  case 24: /* attr: CONTROL id  */
#line 177 "config.y"
                                        { addattr(CONTROL, 0);	}
#line 1407 "y.tab.c"
    break;

  case 25: /* id: IDENT  */
#line 180 "config.y"
                      { yyval = 0; getattrid(yytext); }
#line 1413 "y.tab.c"
    break;

  case 26: /* number: INTEGER  */
#line 183 "config.y"
                        { yyval = config_atoi(yytext, yyleng); }
#line 1419 "y.tab.c"
    break;

  case 27: /* devices: %empty  */
#line 193 "config.y"
                              { doing = "interface types"; }
#line 1425 "y.tab.c"
    break;

  case 31: /* dname: IDENT  */
#line 203 "config.y"
                      { newdev(yytext); }
#line 1431 "y.tab.c"
    break;

  case 33: /* devisid: IDENT  */
#line 209 "config.y"
                      { devisid(yytext); }
#line 1437 "y.tab.c"
    break;

  case 35: /* devonid: IDENT  */
#line 215 "config.y"
                      { devonid(yytext); }
#line 1443 "y.tab.c"
    break;


#line 1447 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 218 "config.y"


#include "lex.yy.c"


/************************************************************************/
/*									*/
/* main  -  main program: parse arguments, invoke the parser, and	*/
/*		write the conf.h and conf.c files			*/
/*									*/
/************************************************************************/


int main(int argc, char **argv) {
	int n, i, j, l, fcount;
	struct dev_ent *s;
	int   verbose = 0;
	char *p;
	int  c;

	if ( argc > 1 && (strncmp("-v", argv[1], 2) == 0) ) {
		argc--;
		argv++;
		verbose++;
	}

	if ( argc > 4 ) {
		fprintf(stderr, "use: config [-v] [input_file] [conf.c] [conf.h]\n");
		exit(1);
	}

	if (verbose) { printf("Opening input file...\n"); }

	if (argc >= 2) {
		if (freopen(argv[1], "r", stdin) == NULL) {
			fprintf(stderr, "Can't open %s\n", argv[1]);
			exit(1);
		}
	}
	else {	/* try to open Configuration file */
		if (freopen(INFILE, "r", stdin) == NULL) {
			fprintf(stderr, "Can't open %s\n", INFILE);
			exit(1);
		}
	}

	/****************************************************************/
	/*								*/
	/*		Parse the Configuration file			*/
	/*								*/
	/****************************************************************/


	if (verbose) { printf("Parsing configuration specs...\n"); }

	if ( (n = yyparse()) != 0 ) { exit(n); }

	/* Open conf.h and conf.c for writing */

	if (verbose) { printf("Opening output files...\n"); }

	if (argc >= 3) {
		if ( (confc = fopen(argv[2],"w") ) == NULL) {
			fprintf(stderr, "Can't write on %s\n", argv[2]);
			exit(1);
		}
	}
	else { 	/* try to open conf.c file */
		if ( (confc = fopen(CONFC,"w") ) == NULL) {
			fprintf(stderr, "Can't write on %s\n", CONFC);
			exit(1);
		}
	}

	if (argc >= 4) {
		if ( (confh = fopen(argv[3],"w") ) == NULL) {
			fprintf(stderr, "Can't write on %s\n", argv[3]);
			exit(1);
		}
	}
	else { 	/* try to open conf.h file */
		if ( (confh = fopen(CONFH,"w") ) == NULL) {
			fprintf(stderr, "Can't write on %s\n", CONFH);
			exit(1);
		}
	}

	/****************************************************************/
	/*								*/
	/*			produce conf.h				*/
	/*								*/
	/****************************************************************/


	fprintf(confh, "/* conf.h (GENERATED FILE; DO NOT EDIT) */\n\n");

	if (verbose) { printf("Writing output...\n"); }

	fprintf(confh, "/* Device switch table declarations */\n\n");

	for (i = 0; (p = devstab[i]) != NULL; i++) {
		fprintf(confh, "%s\n", p);
	}

	fprintf(confh, "\n");

	/* write device declarations and definitions */

	fprintf(confh, "/* Device name definitions */\n\n");
	for (i = 0; i<ndevs; i++) {
		s = &devs[i];
		fprintf(confh, "#define %-20s%2d\t/* type %-8s */\n",
			s->name, i, s->tname);
	}
	fprintf(confh, "\n");

	/* write count of device types */

	fprintf(confh, "/* Control block sizes */\n\n");
	for (i = 0; i < ntypes;  i++) {
		s = &dtypes[i];
		if (s->minor > 0) {
			fprintf(confh, "#define\tN%s\t%d\n",
				s->tname, s->minor);
		}
	}

	fprintf(confh, "\n");

	if (ndevs > 0) { fprintf(confh, "#define NDEVS %d\n", ndevs); }

	/* Copy definitions to output */

	if (brkcount >= 4 && verbose) {
		printf("Copying definitions to %s...\n", CONFH);
	}

	if (brkcount >= 2) {
		while ( (c = input()) > 0) {	/* lex input function	*/
			putc(c, confh);
		}
	}
	fclose(confh);


	/****************************************************************/
	/*								*/
	/*			produce conf.c				*/
	/*								*/
	/****************************************************************/


	fprintf(confc, "/* conf.c (GENERATED FILE; DO NOT EDIT) */\n\n");
	fprintf(confc, "#include <xinu.h>\n\n");
	fprintf(confc, "\n");

	fprintf(confc, "extern\tdevcall\tioerr(void);\n");
	fprintf(confc, "extern\tdevcall\tionull(void);\n\n");

	/* produce devtab (giant I/O switch table) */
	fprintf(confc, "/* Device independent I/O switch */\n\n");
	if (ndevs > 0)
	{
		fprintf(confc, "struct	dentry	devtab[NDEVS] =\n{\n");
		fprintf(confc, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
			"/**",
			" * Format of entries is:",
			" * dev-number, minor-number, dev-name,",
			" * init, open, close,",
			" * read, write, seek,",
			" * getc, putc, control,",
			" * dev-csr-address, intr-handler, irq",
			" */");
	}

	for (i=0; i<ndevs; i++) {
		s = &devs[i];
		fprintf(confc, "/* %s is %s */\n", s->name, s->tname);
		fprintf(confc, "\t{ %d, %d, \"%s\",\n", i, s->minor, s->name);
		fprintf(confc, "\t  (void *)%s, (void *)%s, (void *)%s,\n",
			s->init, s->open, s->close);
		fprintf(confc, "\t  (void *)%s, (void *)%s, (void *)%s,\n",
			s->read, s->write, s->seek);
		fprintf(confc, "\t  (void *)%s, (void *)%s, (void *)%s,\n",
			s->getc, s->putc, s->control);
		fprintf(confc, "\t  (void *)0x%x, (void *)%s, %d }",
			s->csr, s->intr, s->irq);
		if (i< ndevs-1) {
			fprintf(confc, ",\n\n");
		} else {
			fprintf(confc, "\n};");
		}
	}

	/* we must guarantee conf.c written later than conf.h for make */
	fprintf(confc, "\n");
	fclose(confc);

	/* finish up and write report for user if requested */
	if (verbose) {
		printf("Configuration complete. Number of devs=%d:\n", ndevs);
		for (i=0; i<ndevs; i++) {
			s = &devs[i];
			printf("Device %s (on %s)\n", s->name, s->ison);
			printf("    csr=0x%04x, irq=0x%04x, minor=%d\n",
				s->csr, s->irq, s->minor);
		}
	}
}

/************************************************************************/
/*									*/
/* addattr - add a new attribute spec to current type/device description*/
/*		tok: token type (attribute type)			*/
/*		val: symbol number of value				*/
/*									*/
/************************************************************************/

void	addattr(int tok, int val) {
	struct dev_ent *s;
	char *c;

	if (brkcount == 0) {
		/* Doing types */
		s = &dtypes[ntypes-1];
	} else {
		/* Doing devices */
		s = &devs[ndevs-1];
	}

	switch (tok) {
	case CSR:	s->csr = val;			break;
	case IRQ:	s->irq = val;			break;
	case INTR:	strcpy(s->intr, saveattrid);	break;
	case READ:	strcpy(s->read, saveattrid);	break;
	case WRITE:	strcpy(s->write,saveattrid);	break;
	case GETC:	strcpy(s->getc, saveattrid);	break;
	case PUTC:	strcpy(s->putc, saveattrid);	break;
	case OPEN:	strcpy(s->open, saveattrid);	break;
	case CLOSE:	strcpy(s->close,saveattrid);	break;
	case INIT:	strcpy(s->init, saveattrid);	break;
	case SEEK:	strcpy(s->seek, saveattrid);	break;
	case CONTROL:	strcpy(s->control,saveattrid);	break;
	default:	fprintf(stderr, "Internal error 1\n");
	}
}


/************************************************************************/
/*									*/
/* addton -- add an "on XXX" to the current type			*/
/*									*/
/************************************************************************/

int	addton(char *tonid) {
	int	currtype;		/* The current type		*/

	if (strlen(tonid) >= MAXNAME) {
		fprintf(stderr,"string %s is too long on line %d\n",
				tonid, linectr);
		exit(1);
	}
	currtype = ntypes - 1;
	strcpy(dtypes[currtype].ison, tonid);

	return currtype;
}


/************************************************************************/
/*									*/
/* config_atoi - convert an ascii string of text to an integer,		*/
/*			honoring octal, decimal, and hex		*/
/*									*/
/************************************************************************/

int	config_atoi(char *p, int len) {
	int base, rv;

	if (*p == '0')
	{
		++p;
		--len;
		if (*p == 'x' || *p == 'X')
		{
			++p; --len; /* skip 'x' */
			base = 16;
		}
		else
		{
			base = 8;
		}
	}
	else
	{
		base = 10;
	}

	rv = 0;
	for (; len > 0; ++p, --len)
	{
		rv *= base;
		if      (isdigit(*p)) { rv += *p - '0'; }
		else if (isupper(*p)) { rv += *p - 'A' + 10; }
		else                  { rv += *p - 'a' + 10; }
	}

	return rv;
}

/************************************************************************/
/*									*/
/* devisid -- add an "is XXX" to the current device			*/
/*									*/
/************************************************************************/

void	devisid(char *tname) {
	int	currdev;		/* The current device		*/
	int	i;

	if (strlen(tname) >= MAXNAME) {
		fprintf(stderr,"string %s is too long on line %d\n",
				tname, linectr);
		exit(1);
	}
	/* Verify the type exists */

	for (i=0; i<ntypes; i++) {
		if (strcmp(tname, dtypes[i].tname) == 0) {
			break;
		}
	}
	if (i >= ntypes) {
		fprintf(stderr, "Illegal type name %s on line %d\n",
				tname, linectr);
		exit(1);
	}
	currdev = ndevs - 1;
	strcpy(devs[currdev].tname, tname);

	return;
}


/************************************************************************/
/*									*/
/* devonid -- add an "on XXX" to the current device, lookup the type,	*/
/*		and copy attributes into the device from the type	*/
/*									*/
/************************************************************************/

void	devonid(char *onname) {
	int	currdev;		/* The current device		*/
	int	i;
	struct	dev_ent	*dptr;		/* Pointer to current device	*/
	struct	dev_ent	*tptr;		/* Pointer to a type		*/
	char	tmp[MAXNAME];		/* Holds the device name during	*/
					/*   copy			*/

	if (strlen(onname) >= MAXNAME) {
		fprintf(stderr,"string %s is too long on line %d\n",
				onname, linectr);
		exit(1);
	}
	if (ndevs <=0) {
		fprintf(stderr,"Internal error 3\n");
		exit(1);
	}
	currdev = ndevs - 1;
	dptr = &devs[currdev];

	strcpy(dptr->ison, onname);

	/* Lookup the device type */

	for (i=0; i<ntypes; i++) {
		tptr = &dtypes[i];
		if ( (strcmp(dptr->tname,tptr->tname) == 0 ) &&
		     (strcmp(dptr->ison, tptr->ison)  == 0 )  ){

			/* The specified type matches the ith entry, so	*/
			/*  set all attributes equal to the ones in the	*/
			/*  type definition.				*/

			strcpy(tmp, dptr->name);
			bcopy (tptr, dptr, sizeof(struct dev_ent));
			/* Increment the minor device number for the	*/
			/*  next time the type is used			*/
			tptr->minor++;
			strcpy(dptr->name, tmp);
			return;
		}
	}

	fprintf(stderr, "Ileagal device specification on line %d\n", linectr);
	exit(1);
}


/************************************************************************/
/*									*/
/* getattrid -- pick up and save the attribute string from an id	*/
/*									*/
/************************************************************************/

void	getattrid(char *str) {

	if (strlen(str) >= MAXNAME) {
		fprintf(stderr,"atribute string %s is too long on line %d\n",
				str, linectr);
		exit(1);
	}
	strcpy(saveattrid, str);
	return;
}


/************************************************************************/
/*									*/
/* newdev -- allocate an entry in devs, initialize, and fill in the name*/
/*									*/
/************************************************************************/

void	newdev(char *name) {

	struct	dev_ent	*dptr;		/* Ptr. to an entry in devs	*/
	int	i;

	if (ndevs >= NDEVS) {
		fprintf(stderr,"Too many devices on line %d", linectr);
		exit(1);
	}
	if (strlen(name) >= MAXNAME) {
		fprintf(stderr,"Device name %s is too long on line %d\n",
				name, linectr);
		exit(1);
	}

	/* Verify that the device name is unique */

	for (i=0; i<ndevs; i++) {
		if (strcmp(name, devs[i].name) == 0) {
			fprintf(stderr, "Duplicate device name %s on line %d\n",
				name, linectr);
			exit(1);
		}
	}

	dptr = &devs[ndevs];

	/* Initialize fields in the entry */

	bzero((void *)dptr, sizeof(struct dev_ent));
	strcpy(dptr->name,	 name);
	ndevs++;
	return;
}


/************************************************************************/
/*									*/
/* newtype -- allocate an entry in the type array and fill in the name	*/
/*									*/
/************************************************************************/

int	newtype(char *name) {

	struct	dev_ent	*dptr;		/* Ptr. to an entry in dtypes	*/
	int	i;			/* Index into the type table	*/

	if (ntypes >= NTYPES) {
		fprintf(stderr,"Too many types on line %d", linectr);
		exit(1);
	}
	if (strlen(name) >= MAXNAME) {
		fprintf(stderr,"Type name %s is too long on line %d\n",
				name, linectr);
		exit(1);
	}

	/* Verify that the type name is unique */

	for (i=0; i<ntypes; i++) {
		if (strcmp(name, dtypes[i].tname) == 0) {
			fprintf(stderr, "Duplicate type name %s on line %d\n",
				name, linectr);
			exit(1);
		}
	}

	dptr = &dtypes[ntypes];

	/* Initialize fields in the entry */

	bzero((void *)dptr, sizeof(struct dev_ent));
	strcpy(dptr->tname,	 name);
	strncpy(dptr->intr,	"ioerr", 5);
	strncpy(dptr->init,	"ioerr", 5);
	strncpy(dptr->open,	"ioerr", 5);
	strncpy(dptr->close,	"ioerr", 5);
	strncpy(dptr->read,	"ioerr", 5);
	strncpy(dptr->write,	"ioerr", 5);
	strncpy(dptr->control,	"ioerr", 5);
	strncpy(dptr->seek,	"ioerr", 5);
	strncpy(dptr->getc,	"ioerr", 5);
	strncpy(dptr->putc,	"ioerr", 5);

	return ntypes++;
}


/************************************************************************/
/*									*/
/* yyerror - print an error message with the line number		*/
/*									*/
/************************************************************************/

void yyerror(char *s) {

	fprintf(stderr, "Syntax error in %s on line %d\n", doing, linectr);
}
