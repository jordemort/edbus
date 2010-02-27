#ifndef E_OFONO_H
#define E_OFONO_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <Eina.h>
#include <Ecore.h>
#include <E_DBus.h>

#ifdef EAPI
#undef EAPI
#endif
#ifdef _MSC_VER
# ifdef BUILDING_DLL
#  define EAPI __declspec(dllexport)
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

  /* Ecore Events */
  extern int E_OFONO_EVENT_ELEMENT_IN;
  extern int E_OFONO_EVENT_ELEMENT_OUT;
  extern int E_OFONO_EVENT_ELEMENT_ADD;
  extern int E_OFONO_EVENT_ELEMENT_DEL;
  extern int E_OFONO_EVENT_ELEMENT_UPDATED;

  typedef struct _E_Ofono_Element E_Ofono_Element;

  struct _E_Ofono_Element
  {
     const char *path;
     const char *interface;
     E_DBus_Signal_Handler *signal_handler;
     Eina_Inlist *props;

     /* private */
     struct {
	Eina_Inlist *properties_get;
	Eina_Inlist *property_set;
	Eina_Inlist *agent_register;
	Eina_Inlist *agent_unregister;
     } _pending;
     struct {
	Ecore_Idler *changed;
     } _idler;
     Eina_Inlist *_listeners;
     int _references;
  };

  /* General Public API */
  EAPI unsigned int e_ofono_system_init(E_DBus_Connection *edbus_conn) EINA_ARG_NONNULL(1);
  EAPI unsigned int e_ofono_system_shutdown(void);

  /* Manager Methods */
  EAPI E_Ofono_Element *e_ofono_manager_get(void) EINA_WARN_UNUSED_RESULT;

  /* Modem Methods */
  EAPI bool e_ofono_modem_powered_get(E_Ofono_Element *element, Eina_Bool *powered);
  EAPI bool e_ofono_modem_powered_set(E_Ofono_Element *element, Eina_Bool powered, E_DBus_Method_Return_Cb cb, const void *data);
  EAPI bool e_ofono_modem_name_get(E_Ofono_Element *element, const char **name);

  /* NetworkRegistration Methods */
  EAPI bool e_ofono_netreg_mode_get(E_Ofono_Element *element, const char **mode);
  EAPI bool e_ofono_netreg_status_get(E_Ofono_Element *element, const char **status);
  EAPI bool e_ofono_netreg_operator_get(E_Ofono_Element *element, const char **op);
  EAPI bool e_ofono_netreg_strength_get(E_Ofono_Element *element, char *strength);

  /* Low-Level API:
   *
   * Should just be used to work around problems until proper solution
   * is made into e_ofono.
   */
  EAPI bool e_ofono_manager_sync_elements(void);

  EAPI bool e_ofono_elements_get_all(unsigned int *count, E_Ofono_Element ***p_elements) EINA_ARG_NONNULL(1, 2) EINA_WARN_UNUSED_RESULT;
  EAPI bool e_ofono_elements_get_all_type(const char *type, unsigned int *count, E_Ofono_Element ***p_elements) EINA_ARG_NONNULL(1, 2, 3) EINA_WARN_UNUSED_RESULT;
  EAPI E_Ofono_Element *e_ofono_element_get(const char *path, const char *interface);

  EAPI void e_ofono_element_listener_add(E_Ofono_Element *element, void (*cb)(void *data, const E_Ofono_Element *element), const void *data, void (*free_data)(void *data)) EINA_ARG_NONNULL(1, 2);
  EAPI void e_ofono_element_listener_del(E_Ofono_Element *element, void (*cb)(void *data, const E_Ofono_Element *element), const void *data) EINA_ARG_NONNULL(1, 2);

  EAPI int e_ofono_element_ref(E_Ofono_Element *element) EINA_ARG_NONNULL(1);
  EAPI int e_ofono_element_unref(E_Ofono_Element *element) EINA_ARG_NONNULL(1);

  EAPI void e_ofono_element_print(FILE *fp, const E_Ofono_Element *element) EINA_ARG_NONNULL(1, 2);


  EAPI bool e_ofono_element_properties_sync(E_Ofono_Element *element) EINA_ARG_NONNULL(1);
  EAPI bool e_ofono_element_properties_sync_full(E_Ofono_Element *element, E_DBus_Method_Return_Cb cb, const void *data) EINA_ARG_NONNULL(1);

  EAPI bool e_ofono_element_property_set(E_Ofono_Element *element, const char *prop, int type, const void *value) EINA_ARG_NONNULL(1, 2) EINA_WARN_UNUSED_RESULT;
  EAPI bool e_ofono_element_property_set_full(E_Ofono_Element *element, const char *prop, int type, const void *value, E_DBus_Method_Return_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2) EINA_WARN_UNUSED_RESULT;
  EAPI bool e_ofono_element_property_dict_set_full(E_Ofono_Element *element, const char *prop, const char *key, int type, const void *value, E_DBus_Method_Return_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2, 3) EINA_WARN_UNUSED_RESULT;

  EAPI void e_ofono_element_properties_list(const E_Ofono_Element *element, bool (*cb)(void *data, const E_Ofono_Element *element, const char *name, int type, const void *value), const void *data) EINA_ARG_NONNULL(1, 2);

  EAPI bool e_ofono_element_property_type_get_stringshared(const E_Ofono_Element *element, const char *name, int *type) EINA_ARG_NONNULL(1, 2, 3) EINA_WARN_UNUSED_RESULT;
  EAPI bool e_ofono_element_property_type_get(const E_Ofono_Element *element, const char *name, int *type) EINA_ARG_NONNULL(1, 2, 3) EINA_WARN_UNUSED_RESULT;
  EAPI bool e_ofono_element_property_dict_get_stringshared(const E_Ofono_Element *element, const char *dict_name, const char *key_name, int *type, void *value) EINA_ARG_NONNULL(1, 2, 4) EINA_WARN_UNUSED_RESULT;
  EAPI bool e_ofono_element_property_get_stringshared(const E_Ofono_Element *element, const char *name, int *type, void *value) EINA_ARG_NONNULL(1, 2, 4) EINA_WARN_UNUSED_RESULT;
  EAPI bool e_ofono_element_property_get(const E_Ofono_Element *element, const char *name, int *type, void *value) EINA_ARG_NONNULL(1, 2, 4) EINA_WARN_UNUSED_RESULT;

#ifdef __cplusplus
}
#endif
#endif /* E_OFONO_H */