#include "lector_archivo_escena.hpp"
#include "vector.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// Clase fixture para crear archivos temporales
class LectorEscenaTest : public ::testing::Test {
protected:
  string archivo_temporal;

  void SetUp() override { archivo_temporal = "test_escena_temp.txt"; }

  void TearDown() override {
    if (filesystem::exists(archivo_temporal)) {
      filesystem::remove(archivo_temporal);
    }
  }

  void crear_archivo(string const & contenido) {
    ofstream archivo(archivo_temporal);
    archivo << contenido;
    archivo.close();
  }
};

// Tests básicos de materiales

TEST_F(LectorEscenaTest, MaterialMateBasico) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "sphere: 0.0 0.0 0.0 1.0 rojo\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  ASSERT_NO_THROW(leer_escena(archivo_temporal, esferas, cilindros));
  ASSERT_EQ(esferas.size(), 1);
  EXPECT_EQ(esferas[0].tipo_mat, MATE);
  EXPECT_DOUBLE_EQ(esferas[0].reflectancia.getX(), 1.0);
  EXPECT_DOUBLE_EQ(esferas[0].reflectancia.getY(), 0.0);
  EXPECT_DOUBLE_EQ(esferas[0].reflectancia.getZ(), 0.0);
}

TEST_F(LectorEscenaTest, MaterialMetalBasico) {
  crear_archivo("metal: plata 0.9 0.9 0.9 0.1\n"
                "sphere: 1.0 0.0 0.0 2.0 plata\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  ASSERT_NO_THROW(leer_escena(archivo_temporal, esferas, cilindros));
  ASSERT_EQ(esferas.size(), 1);
  EXPECT_EQ(esferas[0].tipo_mat, METAL);
  EXPECT_DOUBLE_EQ(esferas[0].reflectancia.getX(), 0.9);
  EXPECT_DOUBLE_EQ(esferas[0].difusion, 0.1);
}

TEST_F(LectorEscenaTest, MaterialRefractivoBasico) {
  crear_archivo("refractive: vidrio 1.5\n"
                "sphere: 0.0 1.0 0.0 0.5 vidrio\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  ASSERT_NO_THROW(leer_escena(archivo_temporal, esferas, cilindros));
  ASSERT_EQ(esferas.size(), 1);
  EXPECT_EQ(esferas[0].tipo_mat, REFRACTIVO);
  EXPECT_DOUBLE_EQ(esferas[0].indice_refraccion, 1.5);
}

// Tests de esferas

TEST_F(LectorEscenaTest, EsferaConParametrosCorrectos) {
  crear_archivo("matte: azul 0.0 0.0 1.0\n"
                "sphere: 2.0 3.0 4.0 1.5 azul\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  ASSERT_NO_THROW(leer_escena(archivo_temporal, esferas, cilindros));
  ASSERT_EQ(esferas.size(), 1);
  EXPECT_DOUBLE_EQ(esferas[0].centro.getX(), 2.0);
  EXPECT_DOUBLE_EQ(esferas[0].centro.getY(), 3.0);
  EXPECT_DOUBLE_EQ(esferas[0].centro.getZ(), 4.0);
  EXPECT_DOUBLE_EQ(esferas[0].radio, 1.5);
}

TEST_F(LectorEscenaTest, VariasEsferas) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "metal: oro 1.0 0.84 0.0 0.05\n"
                "sphere: 0.0 0.0 0.0 1.0 rojo\n"
                "sphere: 5.0 0.0 0.0 2.0 oro\n"
                "sphere: -3.0 2.0 1.0 0.5 rojo\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  ASSERT_NO_THROW(leer_escena(archivo_temporal, esferas, cilindros));
  EXPECT_EQ(esferas.size(), 3);
  EXPECT_EQ(esferas[0].tipo_mat, MATE);
  EXPECT_EQ(esferas[1].tipo_mat, METAL);
  EXPECT_EQ(esferas[2].tipo_mat, MATE);
}

// Tests de cilindros

TEST_F(LectorEscenaTest, CilindroConParametrosCorrectos) {
  crear_archivo("matte: verde 0.0 1.0 0.0\n"
                "cylinder: 1.0 2.0 3.0 0.5 0.0 5.0 0.0 verde\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  ASSERT_NO_THROW(leer_escena(archivo_temporal, esferas, cilindros));
  ASSERT_EQ(cilindros.size(), 1);
  EXPECT_DOUBLE_EQ(cilindros[0].centro.getX(), 1.0);
  EXPECT_DOUBLE_EQ(cilindros[0].centro.getY(), 2.0);
  EXPECT_DOUBLE_EQ(cilindros[0].centro.getZ(), 3.0);
  EXPECT_DOUBLE_EQ(cilindros[0].radio, 0.5);
  EXPECT_DOUBLE_EQ(cilindros[0].eje.getX(), 0.0);
  EXPECT_DOUBLE_EQ(cilindros[0].eje.getY(), 5.0);
  EXPECT_DOUBLE_EQ(cilindros[0].eje.getZ(), 0.0);
}

TEST_F(LectorEscenaTest, EsferasYCilindros) {
  crear_archivo("matte: blanco 1.0 1.0 1.0\n"
                "metal: cobre 0.72 0.45 0.2 0.2\n"
                "sphere: 0.0 0.0 0.0 1.0 blanco\n"
                "cylinder: 0.0 0.0 0.0 0.3 1.0 0.0 0.0 cobre\n"
                "sphere: 3.0 0.0 0.0 0.5 cobre\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  ASSERT_NO_THROW(leer_escena(archivo_temporal, esferas, cilindros));
  EXPECT_EQ(esferas.size(), 2);
  EXPECT_EQ(cilindros.size(), 1);
}

// Tests de líneas vacías y espacios

TEST_F(LectorEscenaTest, LineasVacias) {
  crear_archivo("\n"
                "matte: rojo 1.0 0.0 0.0\n"
                "\n"
                "\n"
                "sphere: 0.0 0.0 0.0 1.0 rojo\n"
                "\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  ASSERT_NO_THROW(leer_escena(archivo_temporal, esferas, cilindros));
  EXPECT_EQ(esferas.size(), 1);
}

TEST_F(LectorEscenaTest, EspaciosEnBlanco) {
  crear_archivo("  matte:  rojo  1.0  0.0  0.0  \n"
                "  sphere:  0.0  0.0  0.0  1.0  rojo  \n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  ASSERT_NO_THROW(leer_escena(archivo_temporal, esferas, cilindros));
  EXPECT_EQ(esferas.size(), 1);
}

// Tests de errores - Archivo no existe

TEST_F(LectorEscenaTest, ArchivoNoExiste) {
  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena("archivo_inexistente.txt", esferas, cilindros), runtime_error);
}

// Tests de errores - Etiquetas inválidas

TEST_F(LectorEscenaTest, EtiquetaSinDosP) {
  crear_archivo("matte rojo 1.0 0.0 0.0\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, EtiquetaDesconocida) {
  crear_archivo("desconocido: datos\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

// Tests de errores - Materiales

TEST_F(LectorEscenaTest, MaterialMateFaltanParametros) {
  crear_archivo("matte: rojo 1.0 0.0\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, MaterialMetalFaltanParametros) {
  crear_archivo("metal: plata 0.9 0.9\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, MaterialRefractivoFaltanParametros) {
  crear_archivo("refractive: vidrio\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, MaterialDuplicado) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "matte: rojo 0.5 0.0 0.0\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, DatosExtraEnMaterial) {
  crear_archivo("matte: rojo 1.0 0.0 0.0 extra\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

// Tests de errores - Esferas

TEST_F(LectorEscenaTest, EsferaFaltanParametros) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "sphere: 0.0 0.0 0.0 1.0\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, EsferaRadioNegativo) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "sphere: 0.0 0.0 0.0 -1.0 rojo\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, EsferaRadioCero) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "sphere: 0.0 0.0 0.0 0.0 rojo\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, EsferaMaterialNoExiste) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "sphere: 0.0 0.0 0.0 1.0 azul\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, EsferaDatosExtra) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "sphere: 0.0 0.0 0.0 1.0 rojo extra\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

// Tests de errores - Cilindros

TEST_F(LectorEscenaTest, CilindroFaltanParametros) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "cylinder: 0.0 0.0 0.0 1.0 0.0 1.0\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, CilindroRadioNegativo) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "cylinder: 0.0 0.0 0.0 -0.5 0.0 1.0 0.0 rojo\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, CilindroRadioCero) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "cylinder: 0.0 0.0 0.0 0.0 0.0 1.0 0.0 rojo\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, CilindroMaterialNoExiste) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "cylinder: 0.0 0.0 0.0 0.5 0.0 1.0 0.0 verde\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

TEST_F(LectorEscenaTest, CilindroDatosExtra) {
  crear_archivo("matte: rojo 1.0 0.0 0.0\n"
                "cylinder: 0.0 0.0 0.0 0.5 0.0 1.0 0.0 rojo extra\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}

// Tests de casos complejos

TEST_F(LectorEscenaTest, EscenaCompleja) {
  crear_archivo("matte: suelo 0.5 0.5 0.5\n"
                "metal: oro 1.0 0.84 0.0 0.1\n"
                "refractive: vidrio 1.5\n"
                "sphere: 0.0 -100.5 0.0 100.0 suelo\n"
                "sphere: -1.0 0.0 0.0 0.5 oro\n"
                "sphere: 0.0 0.0 0.0 0.5 vidrio\n"
                "cylinder: 1.0 -0.5 0.0 0.2 0.0 1.0 0.0 oro\n"
                "sphere: 1.5 0.2 0.5 0.3 oro\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  ASSERT_NO_THROW(leer_escena(archivo_temporal, esferas, cilindros));
  EXPECT_EQ(esferas.size(), 4);
  EXPECT_EQ(cilindros.size(), 1);

  // Verificar que los materiales se asignaron correctamente
  EXPECT_EQ(esferas[0].tipo_mat, MATE);
  EXPECT_EQ(esferas[1].tipo_mat, METAL);
  EXPECT_EQ(esferas[2].tipo_mat, REFRACTIVO);
  EXPECT_EQ(cilindros[0].tipo_mat, METAL);
}

TEST_F(LectorEscenaTest, MaterialesAntesDeUsarse) {
  crear_archivo("sphere: 0.0 0.0 0.0 1.0 rojo\n"
                "matte: rojo 1.0 0.0 0.0\n");

  vector<esfera> esferas;
  vector<cilindro> cilindros;

  EXPECT_THROW(leer_escena(archivo_temporal, esferas, cilindros), runtime_error);
}
