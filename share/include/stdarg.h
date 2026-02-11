/*	Macros for accessing variable arguments */

typedef char *va_list;

#define va_start(ap, parmn) (ap = (char *)&parmn + sizeof(parmn))

#define va_arg(ap, type) ((ap += sizeof(type)), (*(type *)(ap - sizeof(type))))

#define va_end(ap)