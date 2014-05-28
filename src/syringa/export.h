#ifndef EXPORT_H
#define EXPORT_H

#ifdef SYRINGA_EXPORTS
#define SYRINGA_API __declspec(dllexport)
#else
#define SYRINGA_API __declspec(dllimport)
#endif

#endif // EXPORT_H
